// Written by tools/surface.py: every constant of mcpplibs.mwrap.protobuf.macros against the macro of the same
// name, and GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG against its mapping. The headers come first; each macro is
// taken, then set aside: while it is defined it would replace the qualified name too.
#include <mwrap/protobuf/macros.hpp>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace header {
inline constexpr auto google_protobuf_version = GOOGLE_PROTOBUF_VERSION;
inline constexpr auto google_protobuf_version_suffix = GOOGLE_PROTOBUF_VERSION_SUFFIX;
inline constexpr auto protobuf_oss_version = PROTOBUF_OSS_VERSION;
inline constexpr auto protobuf_oss_version_suffix = PROTOBUF_OSS_VERSION_SUFFIX;
inline constexpr auto protobuf_version = PROTOBUF_VERSION;
inline constexpr auto protobuf_version_suffix = PROTOBUF_VERSION_SUFFIX;
constexpr std::uint32_t make_tag(int number, ::google::protobuf::internal::WireFormatLite::WireType type) {
    return GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(number, type);
}
}  // namespace header

#undef GOOGLE_PROTOBUF_VERSION
#undef GOOGLE_PROTOBUF_VERSION_SUFFIX
#undef PROTOBUF_OSS_VERSION
#undef PROTOBUF_OSS_VERSION_SUFFIX
#undef PROTOBUF_VERSION
#undef PROTOBUF_VERSION_SUFFIX
#undef GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG

#include <gtest/gtest.h>

import mcpplibs.mwrap.protobuf;

namespace macros = mcpplibs::mwrap::protobuf::macros;
using google::protobuf::internal::WireFormatLite;

template <class A, class B>
constexpr bool same(A a, B b) {
    if constexpr (!std::is_same_v<A, B>) return false;
    else if constexpr (std::is_same_v<A, const char*>) return std::string_view(a) == std::string_view(b);
    else return a == b;
}

static_assert(same(macros::GOOGLE_PROTOBUF_VERSION, header::google_protobuf_version), "GOOGLE_PROTOBUF_VERSION");
static_assert(same(macros::GOOGLE_PROTOBUF_VERSION_SUFFIX, header::google_protobuf_version_suffix), "GOOGLE_PROTOBUF_VERSION_SUFFIX");
static_assert(same(macros::PROTOBUF_OSS_VERSION, header::protobuf_oss_version), "PROTOBUF_OSS_VERSION");
static_assert(same(macros::PROTOBUF_OSS_VERSION_SUFFIX, header::protobuf_oss_version_suffix), "PROTOBUF_OSS_VERSION_SUFFIX");
static_assert(same(macros::PROTOBUF_VERSION, header::protobuf_version), "PROTOBUF_VERSION");
static_assert(same(macros::PROTOBUF_VERSION_SUFFIX, header::protobuf_version_suffix), "PROTOBUF_VERSION_SUFFIX");
static_assert(macros::GOOGLE_PROTOBUF_VERSION == 7035001);

TEST(Macros, MakeTagAsTheMacro) {
    for (int number : {1, 2, 15, 16, 2047, 536870911})
        for (auto type : {WireFormatLite::WIRETYPE_VARINT, WireFormatLite::WIRETYPE_FIXED64, WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
                          WireFormatLite::WIRETYPE_START_GROUP, WireFormatLite::WIRETYPE_END_GROUP, WireFormatLite::WIRETYPE_FIXED32})
            EXPECT_EQ(macros::GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(number, type), header::make_tag(number, type));
    static_assert(macros::GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(1, WireFormatLite::WIRETYPE_LENGTH_DELIMITED) == 10);
}
