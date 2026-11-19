// protobuf's API returns and takes Abseil types; this package exports no `absl`
// name, mcpplibs.mwrap.abseil does. With both imported, the status a protobuf
// function returns is that module's absl::Status — one type, one Abseil linked.
//
// GCC 16 workarounds, each seen in CI or locally:
// - the Abseil groups the test names are imported, not the root: the root
//   brings mcpplibs.mwrap.abseil.log, whose import alone stops GCC with an
//   internal compiler error emitting debug information (dwarf2out.cc,
//   gen_enumeration_type_die);
// - the result type is checked on a local variable: a decltype of the call at
//   namespace scope makes GCC report "conflicting type for imported
//   declaration" for descriptor.h's members while merging the modules;
// - failures are reported with ADD_FAILURE rather than GoogleTest's assertion
//   templates, for the same debug-information fault (seen on aarch64).
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.protobuf;
import mcpplibs.mwrap.abseil.status;
import mcpplibs.mwrap.abseil.strings;

namespace pb = google::protobuf;

static_assert(std::is_same_v<absl::string_view, std::string_view>);

TEST(Abseil, ProtobufStatusIsAbseilStatus) {
    pb::Struct object;
    auto bad = pb::json::JsonStringToMessage("{not json", &object);
    static_assert(std::is_same_v<decltype(bad), absl::Status>);
    const absl::Status good = pb::json::JsonStringToMessage(R"({"a":1})", &object);
    std::string report;
    if (bad.ok()) report += "malformed JSON accepted; ";
    if (bad.code() != absl::StatusCode::kInvalidArgument || !absl::IsInvalidArgument(bad)) report += "not InvalidArgument; ";
    if (!good.ok()) report += "valid JSON refused: " + std::string(good.message()) + "; ";
    if (absl::StrCat("fields: ", object.fields_size()) != "fields: 1") report += "StrCat; ";
    if (!report.empty()) ADD_FAILURE() << report;
}
