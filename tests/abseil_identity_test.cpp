// protobuf's API returns and takes Abseil types; this package exports no `absl`
// name, mcpplibs.mwrap.abseil does. With both imported, the status a protobuf
// function returns is that module's absl::Status — one type, one Abseil linked.
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.abseil;
import mcpplibs.mwrap.protobuf;

namespace pb = google::protobuf;

TEST(Abseil, ProtobufStatusIsAbseilStatus) {
    pb::Struct object;
    static_assert(std::is_same_v<decltype(pb::json::JsonStringToMessage(std::string_view{}, &object)), absl::Status>);
    const absl::Status bad = pb::json::JsonStringToMessage("{not json", &object);
    EXPECT_FALSE(bad.ok());
    EXPECT_EQ(bad.code(), absl::StatusCode::kInvalidArgument);
    EXPECT_TRUE(absl::IsInvalidArgument(bad));
    const absl::Status good = pb::json::JsonStringToMessage(R"({"a":1})", &object);
    EXPECT_TRUE(good.ok()) << good;
    static_assert(std::is_same_v<absl::string_view, std::string_view>);
    EXPECT_EQ(absl::StrCat("fields: ", object.fields_size()), "fields: 1");
}
