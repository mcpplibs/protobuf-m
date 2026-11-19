// Written by tools/surface.py from tools/surface.json; tools/check_exports.py audits it. Do not edit by hand.

// mcpplibs.mwrap.protobuf.macros — the version macros of protobuf's headers as constants of the same names in
// mcpplibs::mwrap::protobuf::macros, and GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG as a constexpr function. A module
// does not export macros: the others (GOOGLE_PROTOBUF_VERIFY_VERSION, the platform switches …) come from
// #include <mwrap/protobuf/macros.hpp>, before the import.
module;
#include "detail/prelude.hpp"
#include <google/protobuf/any.h>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/api.pb.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arena_align.h>
#include <google/protobuf/arena_allocation_policy.h>
#include <google/protobuf/arena_cleanup.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/arenaz_sampler.h>
#include <google/protobuf/cpp_edition_defaults.h>
#include <google/protobuf/cpp_features.pb.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor_database.h>
#include <google/protobuf/descriptor_legacy.h>
#include <google/protobuf/descriptor_lite.h>
#include <google/protobuf/descriptor_visitor.h>
#include <google/protobuf/duration.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/empty.pb.h>
#include <google/protobuf/endian.h>
#include <google/protobuf/explicitly_constructed.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/extension_set_inl.h>
#include <google/protobuf/feature_resolver.h>
#include <google/protobuf/field_access_listener.h>
#include <google/protobuf/field_mask.pb.h>
#include <google/protobuf/field_with_arena.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/generated_enum_util.h>
#include <google/protobuf/generated_message_bases.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/generated_message_tctable_decl.h>
#include <google/protobuf/generated_message_tctable_gen.h>
#include <google/protobuf/generated_message_tctable_impl.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/has_bits.h>
#include <google/protobuf/implicit_weak_message.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/internal_feature_helper.h>
#include <google/protobuf/internal_metadata_locator.h>
#include <google/protobuf/internal_visibility.h>
#include <google/protobuf/map.h>
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field.h>
#include <google/protobuf/map_field_lite.h>
#include <google/protobuf/map_type_handler.h>
#include <google/protobuf/message.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/micro_string.h>
#include <google/protobuf/parse_context.h>
#include <google/protobuf/port.h>
#include <google/protobuf/raw_ptr.h>
#include <google/protobuf/reflection.h>
#include <google/protobuf/reflection_internal.h>
#include <google/protobuf/reflection_mode.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/reflection_visit_field_info.h>
#include <google/protobuf/reflection_visit_fields.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/repeated_field_proxy.h>
#include <google/protobuf/repeated_field_proxy_iterator.h>
#include <google/protobuf/repeated_field_proxy_traits.h>
#include <google/protobuf/repeated_ptr_field.h>
#include <google/protobuf/runtime_version.h>
#include <google/protobuf/serial_arena.h>
#include <google/protobuf/service.h>
#include <google/protobuf/source_context.pb.h>
#include <google/protobuf/string_block.h>
#include <google/protobuf/string_member_robber.h>
#include <google/protobuf/struct.pb.h>
#include <google/protobuf/symbol_checker.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/thread_safe_arena.h>
#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/type.pb.h>
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/varint_shuffle.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/wrappers.pb.h>

namespace mwrap_protobuf_macros {
inline constexpr auto value_GOOGLE_PROTOBUF_VERSION = GOOGLE_PROTOBUF_VERSION;
inline constexpr auto value_GOOGLE_PROTOBUF_VERSION_SUFFIX = GOOGLE_PROTOBUF_VERSION_SUFFIX;
inline constexpr auto value_PROTOBUF_OSS_VERSION = PROTOBUF_OSS_VERSION;
inline constexpr auto value_PROTOBUF_OSS_VERSION_SUFFIX = PROTOBUF_OSS_VERSION_SUFFIX;
inline constexpr auto value_PROTOBUF_VERSION = PROTOBUF_VERSION;
inline constexpr auto value_PROTOBUF_VERSION_SUFFIX = PROTOBUF_VERSION_SUFFIX;
}  // namespace mwrap_protobuf_macros

#undef GOOGLE_PROTOBUF_VERSION
#undef GOOGLE_PROTOBUF_VERSION_SUFFIX
#undef PROTOBUF_OSS_VERSION
#undef PROTOBUF_OSS_VERSION_SUFFIX
#undef PROTOBUF_VERSION
#undef PROTOBUF_VERSION_SUFFIX
#undef GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG

export module mcpplibs.mwrap.protobuf.macros;

export namespace mcpplibs::mwrap::protobuf::macros {

inline constexpr auto GOOGLE_PROTOBUF_VERSION = mwrap_protobuf_macros::value_GOOGLE_PROTOBUF_VERSION;
inline constexpr auto GOOGLE_PROTOBUF_VERSION_SUFFIX = mwrap_protobuf_macros::value_GOOGLE_PROTOBUF_VERSION_SUFFIX;
inline constexpr auto PROTOBUF_OSS_VERSION = mwrap_protobuf_macros::value_PROTOBUF_OSS_VERSION;
inline constexpr auto PROTOBUF_OSS_VERSION_SUFFIX = mwrap_protobuf_macros::value_PROTOBUF_OSS_VERSION_SUFFIX;
inline constexpr auto PROTOBUF_VERSION = mwrap_protobuf_macros::value_PROTOBUF_VERSION;
inline constexpr auto PROTOBUF_VERSION_SUFFIX = mwrap_protobuf_macros::value_PROTOBUF_VERSION_SUFFIX;

// wire_format_lite.h: the tag of a field, (number << 3) | wire type.
constexpr std::uint32_t GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(int field_number, ::google::protobuf::internal::WireFormatLite::WireType type) {
    return static_cast<std::uint32_t>((static_cast<std::uint32_t>(field_number) << ::google::protobuf::internal::WireFormatLite::kTagTypeBits) | type);
}

}  // namespace mcpplibs::mwrap::protobuf::macros
