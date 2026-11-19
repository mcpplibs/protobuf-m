// The headers and the import in one translation unit, the way a program with
// generated code is written: the prelude (every header the modules include)
// first, then the import. The module's names are the very same entities as the
// headers': an object made through one is used through the other.
#include <mwrap/protobuf/prelude.hpp>
#include <google/protobuf/timestamp.pb.h>
#include <gtest/gtest.h>
#include <type_traits>

import mcpplibs.mwrap.protobuf;

TEST(HeaderMix, TheSameEntities) {
    ::google::protobuf::Timestamp through_header;
    through_header.set_seconds(10);
    google::protobuf::Any any;
    ASSERT_TRUE(any.PackFrom(through_header));
    google::protobuf::Timestamp back;
    ASSERT_TRUE(any.UnpackTo(&back));
    EXPECT_EQ(::google::protobuf::util::TimeUtil::ToString(back), "1970-01-01T00:00:10Z");
    static_assert(std::is_base_of_v<::google::protobuf::Message, google::protobuf::Timestamp>);
    GOOGLE_PROTOBUF_VERIFY_VERSION;  // a statement macro, from the headers
}
