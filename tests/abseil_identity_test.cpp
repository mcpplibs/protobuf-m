// protobuf's API returns and takes Abseil types; this package exports no `absl`
// name, mcpplibs.mwrap.abseil does. With both imported, the status a protobuf
// function returns is that module's absl::Status — one type, one Abseil linked.
//
// The checks are gathered into one bool and reported with ADD_FAILURE: GCC 16 on
// aarch64 (musl) stops with an internal compiler error (dwarf2out.cc,
// gen_enumeration_type_die) emitting the debug information of GoogleTest's
// AssertionResult template in this translation unit, which imports an
// enumeration-carrying module of Abseil.
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.abseil;
import mcpplibs.mwrap.protobuf;

namespace pb = google::protobuf;

static_assert(std::is_same_v<decltype(pb::json::JsonStringToMessage(std::string_view{}, static_cast<pb::Struct*>(nullptr))), absl::Status>);
static_assert(std::is_same_v<absl::string_view, std::string_view>);

TEST(Abseil, ProtobufStatusIsAbseilStatus) {
    pb::Struct object;
    const absl::Status bad = pb::json::JsonStringToMessage("{not json", &object);
    const absl::Status good = pb::json::JsonStringToMessage(R"({"a":1})", &object);
    std::string report;
    if (bad.ok()) report += "malformed JSON accepted; ";
    if (bad.code() != absl::StatusCode::kInvalidArgument || !absl::IsInvalidArgument(bad)) report += "not InvalidArgument; ";
    if (!good.ok()) report += "valid JSON refused: " + std::string(good.message()) + "; ";
    if (absl::StrCat("fields: ", object.fields_size()) != "fields: 1") report += "StrCat; ";
    if (!report.empty()) ADD_FAILURE() << report;
}
