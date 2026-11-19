// protogen — protoc as an importable build rule: the generic layer of protobuf
// code generation, owned by protobuf.
//
//   import mcpp;
//   import protogen;
//   int main() { return protogen::generate_all() ? 0 : 1; }
//
// Every .proto under proto/ becomes one build-graph edge that runs protoc with
// --cpp_out (C++ messages, `<name>.pb.{h,cc}`), and with any protoc plugin
// given in `options::plugins`; the .pb.cc enter the build, the directory of the
// .pb.h becomes an include directory of the package. protoc is built by mcpp
// for the build machine from the same protobuf release the program links.
//
// The layers, each the next one's defaults (the API of grpc-m's grpcgen, whose
// generic half this is):
//
//   L0  generate_all()                                   the whole directory
//   L1  generate_all({.imports=…, .lite=…, .descriptor_set=…, .protoc_args=…})
//   L2  generate_all({.plugins={my_plugin}})
//   L3  auto e = plan_all(); /* edit */ submit(e);
//
// `generate_all(opt)` IS `submit(plan_all(opt))`. What this rule does not do is
// print command lines: build.ninja has every action's argv
// (`ninja -t commands <output>`); the rule owns which knobs produced it, in
// each edge's description.
export module protogen;

import std;
import mcpp;

namespace protogen::detail {

// protoc does not embed the well-known types: `import
// "google/protobuf/timestamp.proto"` — and Duration, Any, Empty, Struct, which
// real services use constantly — is read from disk. They ship inside the
// protobuf package the consumer already depends on, so the path is derivable
// and nobody has to install anything.
//
// Found by looking for the directory that actually holds descriptor.proto,
// rather than by assuming the tarball's wrap-directory name: that name is a
// packaging artifact and not part of any contract.
std::string well_known_types_dir() {
    namespace fs = std::filesystem;
    const std::string base = mcpp::dep_dir("compat.protobuf");
    if (base.empty()) return {};
    std::error_code ec;
    for (const auto& e : fs::directory_iterator(base, ec)) {
        const fs::path src = e.path() / "src";
        if (fs::exists(src / "google" / "protobuf" / "descriptor.proto", ec))
            return src.generic_string();
    }
    return {};
}

}  // namespace protogen::detail

export namespace protogen {

// ── L2: a protoc plugin ────────────────────────────────────────────────────
//
// One plugin is four things, and the fourth is not optional: mcpp has to know
// which files the edge PRODUCES to put the .cc in the compile set and keep the
// .h out of it. A plugin that cannot say what it emits cannot be a build-graph
// node at all.
struct plugin {
    // The protoc name: drives `--<name>_out=` and `--plugin=protoc-gen-<name>=`.
    std::string name;
    // Absolute path to the generator executable. Empty = unresolved; the rule
    // reports it by name rather than letting protoc fail with its own wording.
    std::string binary;
    // Plugin parameters, joined with ',' and prefixed onto the output dir —
    // protoc's `--x_out=a=1,b=2:<dir>` grammar.
    std::vector<std::string> params;
    // Suffixes appended to the .proto's stem, e.g. ".grpc.pb.cc".
    std::vector<std::string> suffixes;
};

struct options {
    // Where the .proto files live, relative to the consumer's manifest.
    std::string_view proto_dir = "proto";
    // Additional trees to GENERATE from, each also becoming an `-I` root.
    // Names keep their path relative to their own root, so
    // ../shared-proto/common/types.proto emits common/types.pb.{h,cc}.
    //
    // Distinct from `imports` on purpose: protoc emits
    // `#include "common/types.pb.h"` into any file that imports that .proto,
    // so a shared tree reached only through `-I` yields a header nobody
    // produced. Use this when the shared .proto's code is yours to build.
    std::vector<std::string> extra_dirs;
    // Extra `-I` roots, SEARCH ONLY: no code is generated for what they
    // contain (the generated code comes from a package you already link).
    std::vector<std::string> imports;
    // Generate for the lite runtime (`--cpp_out=lite:`): MessageLite, no
    // descriptors or reflection.
    bool lite = false;
    // Also write each .proto's FileDescriptorSet, with its imports, beside the
    // generated code as `<name>.pb.desc` (protoc --descriptor_set_out
    // --include_imports): what a DescriptorPool / DynamicMessage needs at run
    // time without the generated classes.
    bool descriptor_set = false;
    // Escape hatch for protoc flags this rule does not model, e.g.
    // "--experimental_allow_proto3_optional".
    std::vector<std::string> protoc_args;
    // L2: protoc plugins run with the C++ generator, each a build-graph output
    // set of its own. Empty: messages only, and no plugin dependency.
    std::vector<plugin> plugins;
};

// ── L3: a planned edge, before it is submitted ─────────────────────────────
//
// Owns its strings, unlike `mcpp::action` whose id/description are raw
// pointers — a planned edge outlives the expression that built it.
struct edge {
    std::string              id;
    std::string              description;
    std::vector<std::string> command;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

    edge& arg(std::string a)    { command.push_back(std::move(a)); return *this; }
    edge& input(std::string p)  { inputs.push_back(std::move(p));  return *this; }
    edge& output(std::string p) { outputs.push_back(std::move(p)); return *this; }
};

namespace detail {

std::vector<plugin> resolve_plugins(const options& opt) { return opt.plugins; }

// "protoc:echo (+grpc generate_mock_code=true, -I proto -I ../shared/proto)"
//
// The description is what every build prints, so it is where "which knobs
// produced this" belongs. The command itself is in build.ninja; see the module
// header for why the rule does not duplicate it.
std::string describe(const std::string& name,
                     const std::vector<plugin>& plugins,
                     const std::vector<std::string>& userIncs,
                     const std::string& root) {
    std::string s = "protoc:" + name + " (";
    if (plugins.empty()) {
        s += "messages";
    } else {
        bool first = true;
        for (const auto& p : plugins) {
            if (!first) s += " ";
            first = false;
            s += "+" + p.name;
            for (const auto& q : p.params) {
                s += " " + q;
            }
        }
    }
    // Only the roots the AUTHOR chose. The well-known-types directory is
    // always present and is not a knob, and absolute store paths would bury
    // the part worth reading. Rendered relative to the manifest so the string
    // matches what was written in build.mcpp.
    for (const auto& i : userIncs) {
        // Prefer the relative form — it is usually what the author wrote — but
        // only while it stays readable. `lexically_relative` SUCCEEDS for a
        // path far from the manifest and returns a chain of `..` longer than
        // the absolute path it replaced; guarding only on "could not compute"
        // misses that case entirely.
        auto rel = std::filesystem::path(i).lexically_relative(root);
        auto r = rel.generic_string();
        auto shown = (r.empty() || r.starts_with("../..")) ? i : r;
        s += (i == userIncs.front() ? ", -I" : " -I") + shown;
    }
    s += ")";
    return s;
}

}  // namespace detail

// Build the edges for the named .proto files WITHOUT submitting them.
//
// Names are given WITHOUT the extension, relative to `opt.proto_dir` —
// "helloworld" means <proto_dir>/helloworld.proto.
//
// Returns an empty vector after printing a diagnostic.
// ── L3, bottom: a .proto addressed by (root, name) ─────────────────────────
//
// `plan()` and `plan_all()` both funnel here. It is public because it is the
// only form that can express a .proto living somewhere neither `proto_dir` nor
// `extra_dirs` covers — without it, such a project would be back to writing
// its own build.mcpp, which is the cliff this whole design exists to remove.
//
// `name` is relative to `root` and decides the OUTPUT path, so two roots may
// contribute the same relative name only if the author meant it — exactly the
// property protoc's own -I list has.
struct entry { std::string root, name; };

// Plan edges for .proto files given as explicit (root, name) pairs.
std::vector<edge> plan_entries(std::vector<entry> protos, options opt = {}) {
    const std::string root = mcpp::manifest_dir();
    const std::string out  = mcpp::out_dir();
    if (root.empty() || out.empty()) {
        std::println(std::cerr,
            "protogen: no mcpp build context — this runs from build.mcpp");
        return {};
    }
    if (protos.empty()) {
        std::println(std::cerr, "protogen: no .proto files to plan");
        return {};
    }

    // By the qualified name: a project that uses this rule usually depends on
    // mcpplibs.mwrap.protobuf too, whose short name is also `protobuf`.
    const char* protoc = mcpp::dep_bin("compat.protobuf", "protoc");
    if (!protoc || !*protoc) {
        std::println(std::cerr,
            "protogen: no protoc. Declare it on the protobuf package:\n"
            "  mcpplibs.mwrap.protobuf = {{ version = \"35.1\", features = [\"codegen\"] }}\n"
            "  (or compat.protobuf = {{ version = \"35.1\", features = [\"upb\"], tools = [\"protoc\"] }})\n"
            "  On Windows compat.protobuf 35.1 does not build protoc; generate on Linux or macOS.");
        return {};
    }

    auto plugins = detail::resolve_plugins(opt);
    for (const auto& p : plugins) {
        if (!p.binary.empty()) continue;
        std::println(std::cerr,
            "protogen: plugin '{}' has no executable: declare the package that "
            "builds it with `tools = [...]` and pass its mcpp::dep_bin path", p.name);
        return {};
    }

    const std::string wkt = detail::well_known_types_dir();
    if (wkt.empty()) {
        std::println(std::cerr,
            "protogen: cannot locate the well-known .proto files inside the "
            "protobuf package");
        return {};
    }

    auto abs_under_root = [&](const std::string& v) {
        std::filesystem::path p{v};
        return p.is_absolute() ? p.generic_string()
                               : (std::filesystem::path(root) / p)
                                     .lexically_normal().generic_string();
    };
    const std::string protoDir = abs_under_root(std::string(opt.proto_dir));

    // Include roots, in the order protoc sees them: the generation roots (own
    // tree first), then search-only roots, then the well-known types.
    std::vector<std::string> incs{ protoDir };
    for (const auto& d : opt.extra_dirs) incs.push_back(abs_under_root(d));
    for (const auto& i : opt.imports)    incs.push_back(abs_under_root(i));
    incs.push_back(wkt);

    // Every .proto is an input of every action. A .proto that imports a sibling
    // has a real dependency this rule does not parse, and regenerating a little
    // too eagerly is far better than silently stale stubs.
    std::vector<std::string> inputs;
    inputs.reserve(protos.size());
    for (const auto& e : protos)
        inputs.push_back(std::format("{}/{}.proto", e.root, e.name));

    std::vector<edge> edges;
    edges.reserve(protos.size());

    for (const auto& ent : protos) {
        const std::string& name = ent.name;
        const std::string src  = std::format("{}/{}.proto", ent.root, name);
        const std::string base = std::format("{}/{}", out, name);

        edge e;
        e.id          = std::format("protoc:{}", name);
        // Only the author-chosen roots: incs also carries the well-known-types
        // directory, which is constant and not worth printing every build.
        e.description = detail::describe(
            name, plugins,
            std::vector<std::string>(incs.begin(), incs.end() - 1), root);

        e.arg(protoc);
        for (const auto& i : incs) e.arg("-I" + i);
        e.arg(std::string("--cpp_out=") + (opt.lite ? "lite:" : "") + out);
        if (opt.descriptor_set) {
            e.arg("--include_imports");
            e.arg("--descriptor_set_out=" + base + ".pb.desc");
        }

        for (const auto& p : plugins) {
            std::string spec;
            for (const auto& q : p.params) {
                if (!spec.empty()) spec += ",";
                spec += q;
            }
            // protoc's grammar: `--x_out=<params>:<dir>`; the colon is only
            // present when there are params.
            e.arg("--" + p.name + "_out=" + (spec.empty() ? out : spec + ":" + out));
            e.arg("--plugin=protoc-gen-" + p.name + "=" + p.binary);
        }
        for (const auto& a : opt.protoc_args) e.arg(a);
        e.arg(src);

        for (const auto& in : inputs) e.input(in);

        // protoc mirrors the .proto's relative path under --cpp_out and does
        // NOT create the intermediate directories. A flat proto/ never notices;
        // proto/sub/x.proto fails with "No such file or directory" from inside
        // protoc, which reads as a codegen bug rather than a missing mkdir.
        {
            std::error_code mkec;
            std::filesystem::create_directories(
                std::filesystem::path(base).parent_path(), mkec);
        }
        // The headers are declared as outputs because they must be PRODUCED by
        // this edge; mcpp knows a header is not a translation unit and keeps
        // them out of the compile set.
        e.output(base + ".pb.cc").output(base + ".pb.h");
        if (opt.descriptor_set) e.output(base + ".pb.desc");
        for (const auto& p : plugins)
            for (const auto& s : p.suffixes) e.output(base + s);

        edges.push_back(std::move(e));
    }
    return edges;
}

// Every .proto under `opt.proto_dir`, without naming any of them — planned but
// not submitted.
//
// Safe only because the engine can express "my output depends on which files
// are here" (`rerun_if_changed_glob`, mcpp 2026.8.6.2+). Before that, a build
// program that globbed did not re-run when a .proto was added — no declared
// file's hash had changed — so the new file was silently never generated,
// which is worse than making the author list names. That is why this
// repository shipped the explicit list first.
//
// Names keep their subdirectory, so proto/sub/x.proto generates sub/x.pb.cc.
// Build the edges for the named .proto files WITHOUT submitting them.
//
// Names are given WITHOUT the extension, relative to `opt.proto_dir` —
// "helloworld" means <proto_dir>/helloworld.proto. Files under `extra_dirs`
// are not addressable this way; use plan_all() for those.
//
// Returns an empty vector after printing a diagnostic.
std::vector<edge> plan(std::vector<std::string> protos, options opt = {}) {
    const std::string root = mcpp::manifest_dir();
    if (root.empty()) {
        std::println(std::cerr,
            "protogen: no mcpp build context — this runs from build.mcpp");
        return {};
    }
    std::filesystem::path pd{std::string(opt.proto_dir)};
    const std::string base = pd.is_absolute()
        ? pd.generic_string()
        : (std::filesystem::path(root) / pd).lexically_normal().generic_string();
    std::vector<entry> entries;
    entries.reserve(protos.size());
    for (auto& n : protos) entries.push_back({ base, std::move(n) });
    return plan_entries(std::move(entries), std::move(opt));
}

std::vector<edge> plan_all(options opt = {}) {
    namespace fs = std::filesystem;
    const std::string root = mcpp::manifest_dir();
    if (root.empty()) {
        std::println(std::cerr,
            "protogen: no mcpp build context — this runs from build.mcpp");
        return {};
    }
    // Every generation root: the project's own tree, then `extra_dirs`.
    std::vector<std::string> roots{ std::string(opt.proto_dir) };
    for (const auto& d : opt.extra_dirs) roots.push_back(d);

    std::vector<entry> entries;
    for (const auto& r : roots) {
        // One glob per root — a .proto appearing in ANY of them must re-run
        // this program, and only the path SET is fingerprinted (contents are
        // covered by the edges' own inputs).
        const std::string pattern = r + "/**/*.proto";
        mcpp::rerun_if_changed_glob(pattern.c_str());

        fs::path rp{r};
        const fs::path dir = rp.is_absolute() ? rp
                                              : (fs::path(root) / rp).lexically_normal();
        std::error_code ec;
        if (!fs::exists(dir, ec)) {
            std::println(std::cerr,
                "protogen::plan_all(): no '{}' directory under {}", r, root);
            return {};
        }
        std::vector<std::string> names;
        for (auto const& e : fs::recursive_directory_iterator(dir, ec)) {
            if (ec) break;
            if (!e.is_regular_file(ec)) continue;
            if (e.path().extension() != ".proto") continue;
            auto rel = e.path().lexically_relative(dir);
            rel.replace_extension();
            names.push_back(rel.generic_string());
        }
        // Sorted so the declared edge set is stable run to run — an unstable
        // order would churn build.ninja for no reason.
        std::ranges::sort(names);
        for (auto& n : names)
            entries.push_back({ dir.generic_string(), std::move(n) });
    }
    if (entries.empty()) {
        std::println(std::cerr,
            "protogen::plan_all(): no .proto files under {}", opt.proto_dir);
        return {};
    }
    return plan_entries(std::move(entries), std::move(opt));
}

// Hand the planned edges to mcpp, and declare where the generated headers live.
//
// That include dir is PRIVATE to the consuming package by design — one a
// package's own consumers must see belongs in its manifest, not in a build
// program.
bool submit(const std::vector<edge>& edges) {
    if (edges.empty()) return false;
    for (const auto& e : edges) {
        mcpp::action a;
        a.id          = e.id.c_str();
        a.role        = "source";
        a.description = e.description.c_str();
        for (const auto& c : e.command) a.arg(c.c_str());
        for (const auto& i : e.inputs)  a.input(i.c_str());
        for (const auto& o : e.outputs) a.output(o.c_str());
        a.submit();
    }
    mcpp::include_dir(mcpp::out_dir());
    return true;
}

// Declare one codegen edge per named .proto. `plan` + `submit`, with nothing
// in between — the two-call form exists so a consumer can put something there.
bool generate(std::vector<std::string> protos, options opt = {}) {
    return submit(plan(std::move(protos), std::move(opt)));
}

// Every .proto under `opt.proto_dir`. The form the greeter template uses.
bool generate_all(options opt = {}) {
    return submit(plan_all(std::move(opt)));
}

}  // namespace protogen
