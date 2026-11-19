// Written by tools/surface.py from tools/surface.json; tools/check_exports.py audits it. Do not edit by hand.

// mcpplibs.mwrap.protobuf.util — JSON, MessageDifferencer, TimeUtil, FieldMaskUtil, delimited messages and type resolvers (protobuf 35.1), unchanged:
// each name is a using-declaration of the upstream entity in the namespace upstream declares it in.
module;
#include "detail/prelude.hpp"
#include <google/protobuf/util/delimited_message_util.h>
#include <google/protobuf/util/field_comparator.h>
#include <google/protobuf/util/field_mask_util.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/message_differencer.h>
#include <google/protobuf/util/time_util.h>
#include <google/protobuf/util/type_resolver.h>
#include <google/protobuf/util/type_resolver_util.h>
#include <google/protobuf/json/json.h>
#include "detail/epilogue.hpp"

export module mcpplibs.mwrap.protobuf.util;

export namespace google::protobuf {
// functions and operators (17)
using ::google::protobuf::operator!=;
using ::google::protobuf::operator%;
using ::google::protobuf::operator%=;
using ::google::protobuf::operator*;
using ::google::protobuf::operator*=;
using ::google::protobuf::operator+;
using ::google::protobuf::operator+=;
using ::google::protobuf::operator-;
using ::google::protobuf::operator-=;
using ::google::protobuf::operator/;
using ::google::protobuf::operator/=;
using ::google::protobuf::operator<;
using ::google::protobuf::operator<<;
using ::google::protobuf::operator<=;
using ::google::protobuf::operator==;
using ::google::protobuf::operator>;
using ::google::protobuf::operator>=;
}  // namespace google::protobuf

export namespace google::protobuf::json {
// types (2)
using ::google::protobuf::json::ParseOptions;
using ::google::protobuf::json::PrintOptions;
// functions and operators (8)
using ::google::protobuf::json::BinaryToJsonStream;
using ::google::protobuf::json::BinaryToJsonString;
using ::google::protobuf::json::JsonStreamToMessage;
using ::google::protobuf::json::JsonStringToMessage;
using ::google::protobuf::json::JsonToBinaryStream;
using ::google::protobuf::json::JsonToBinaryString;
using ::google::protobuf::json::MessageToJsonStream;
using ::google::protobuf::json::MessageToJsonString;
}  // namespace google::protobuf::json

export namespace google::protobuf::util {
// types (10)
using ::google::protobuf::util::DefaultFieldComparator;
using ::google::protobuf::util::FieldComparator;
using ::google::protobuf::util::FieldContext;
using ::google::protobuf::util::FieldMaskUtil;
using ::google::protobuf::util::JsonParseOptions;
using ::google::protobuf::util::JsonPrintOptions;
using ::google::protobuf::util::MessageDifferencer;
using ::google::protobuf::util::SimpleFieldComparator;
using ::google::protobuf::util::TimeUtil;
using ::google::protobuf::util::TypeResolver;
// functions and operators (8)
using ::google::protobuf::util::ConvertDescriptorToType;
using ::google::protobuf::util::NewTypeResolverForDescriptorPool;
using ::google::protobuf::util::ParseDelimitedFromCodedStream;
using ::google::protobuf::util::ParseDelimitedFromZeroCopyStream;
using ::google::protobuf::util::SerializeDelimitedToCodedStream;
using ::google::protobuf::util::SerializeDelimitedToFileDescriptor;
using ::google::protobuf::util::SerializeDelimitedToOstream;
using ::google::protobuf::util::SerializeDelimitedToZeroCopyStream;
// names protobuf re-declares from another namespace (6)
using ::google::protobuf::util::BinaryToJsonStream;
using ::google::protobuf::util::BinaryToJsonString;
using ::google::protobuf::util::JsonStringToMessage;
using ::google::protobuf::util::JsonToBinaryStream;
using ::google::protobuf::util::JsonToBinaryString;
using ::google::protobuf::util::MessageToJsonString;
}  // namespace google::protobuf::util
