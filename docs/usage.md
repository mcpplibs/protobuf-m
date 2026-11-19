# Using mcpplibs.mwrap.protobuf

[README](../README.md) · [every exported name](api.md)

`import mcpplibs.mwrap.protobuf;` declares protobuf 35.1's C++ API as its headers do: the
names are the upstream entities, in the namespaces upstream declares them in
(`google::protobuf`, `::io`, `::util`, `::json`, `::compiler`). The library is
`compat.protobuf` 35.1 (libprotobuf and utf8_range) over `compat.abseil` 20250512.1.

## Modules

| Module | Holds |
|---|---|
| `mcpplibs.mwrap.protobuf` | `Message`, `MessageLite`, `Reflection`, `Descriptor` and the other descriptors, `DescriptorPool`, `DynamicMessageFactory`, `Arena`, `RepeatedField`, `RepeatedPtrField`, `Map`, `TextFormat`, `UnknownFieldSet`, the well-known types (`Any`, `Timestamp`, `Duration`, `Struct`, `Value`, `FieldMask`, `Empty`, the wrappers, `Api`, `Type` …) and `descriptor.proto`'s messages; re-exports the four below |
| `mcpplibs.mwrap.protobuf.io` | `CodedInputStream`, `CodedOutputStream`, the zero-copy streams, `Tokenizer`, `ErrorCollector`, `Printer` |
| `mcpplibs.mwrap.protobuf.util` | `json::MessageToJsonString` / `JsonStringToMessage` (also as `util::`), `MessageDifferencer`, `TimeUtil` with the Timestamp/Duration operators, `FieldMaskUtil`, `SerializeDelimitedTo*` / `ParseDelimitedFrom*`, `TypeResolver` |
| `mcpplibs.mwrap.protobuf.compiler` | `Importer`, `DiskSourceTree`, `SourceTree`, `Parser`, `MultiFileErrorCollector` |
| `mcpplibs.mwrap.protobuf.macros` | the version macros as constants, `GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG` as a constexpr function |
| `mcpplibs.mwrap.protobuf.io.gzip` | `GzipInputStream`, `GzipOutputStream`: only with the `gzip` feature (compat.protobuf defines them only then); import it by name |

Names in implementation namespaces (`internal`) are not exported, except their operators,
which apply to public types and are found by argument-dependent lookup. The constants
`kint32max` … `kuint64max` (`static` in the headers) are declared again in
`mcpplibs::mwrap::protobuf`.

## Generated code: the codegen feature and protogen

```toml
[dependencies]
mcpplibs.mwrap.protobuf = { version = "35.1", features = ["codegen"] }
```

`codegen` builds protoc for the build machine from the protobuf release the program links
(so the generator and the runtime cannot differ) and hands the project the `protogen`
rule. `build.mcpp`:

```cpp
import mcpp;
import protogen;
int main() { return protogen::generate_all() ? 0 : 1; }
```

Every `.proto` under `proto/` becomes one build-graph edge; `proto/a/b.proto` generates
`a/b.pb.{h,cc}`, the `.pb.cc` enter the build and the header directory becomes an include
directory. Options (the same names as grpc-m's `grpcgen`, whose generic layer this is):

| Option | Effect |
|---|---|
| `.proto_dir` | the directory scanned (default `proto`) |
| `.extra_dirs` | further trees to generate from, each also an `-I` root |
| `.imports` | `-I` roots searched only (their code comes from elsewhere) |
| `.lite` | `--cpp_out=lite:`: `MessageLite` classes, no descriptors or reflection |
| `.descriptor_set` | also `<name>.pb.desc`, the FileDescriptorSet with imports |
| `.protoc_args` | any other protoc flag |
| `.plugins` | protoc plugins: `protogen::plugin{name, binary, params, suffixes}` |

`generate({"name"})` names files; `plan_all()` returns the edges for inspection or editing
before `submit()`. A gRPC project uses grpc-m's `grpcgen` instead, which adds the gRPC
plugin. On Windows compat.protobuf 35.1 does not build protoc, so `codegen` fails there with
a message; generate on Linux or macOS.

A translation unit that includes generated headers and imports the module includes
`<mwrap/protobuf/prelude.hpp>` first — every header the modules include — then the
generated headers, then imports. With GCC every `#include` must come before the first
`import`, and a translation unit that includes fewer headers than the modules did can read
a declaration of the module as conflicting with its own. Clang accepts both orders.
Generated code declares a top-level namespace `pb` (`pb::CppFeatures`); the module does not
export it, so a module-only translation unit may use `namespace pb = google::protobuf;`.

## Abseil

protobuf's API uses Abseil types: `absl::Status` and `absl::StatusOr` (JSON, TextFormat
errors), `absl::string_view` (= `std::string_view`), `absl::Cord`. No `absl` name is
exported here; a value of these types is used through its members without naming it
(`if (!json::MessageToJsonString(m, &s).ok())`). To name a type, import
`mcpplibs.mwrap.abseil` (the same entities, linked once) or include its header.

## Macros

`mcpplibs::mwrap::protobuf::macros` holds `GOOGLE_PROTOBUF_VERSION`,
`PROTOBUF_VERSION` (7035001) and their `_SUFFIX` strings, and
`GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(number, wire_type)`. The others —
`GOOGLE_PROTOBUF_VERIFY_VERSION` (a statement), the `GOOGLE_PROTOBUF_ARCH_*` switches,
annotations — come from `<mwrap/protobuf/macros.hpp>`, included before the imports.

## Platforms

Linux (x86_64, aarch64), macOS, Windows (x86_64) and the Web (wasm32-emscripten): the
runtime everywhere; `codegen` where protoc builds (not Windows).
