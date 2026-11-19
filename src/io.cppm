// Written by tools/surface.py from tools/surface.json; tools/check_exports.py audits it. Do not edit by hand.

// mcpplibs.mwrap.protobuf.io — coded and zero-copy streams, the tokenizer and the printer (protobuf 35.1), unchanged:
// each name is a using-declaration of the upstream entity in the namespace upstream declares it in.
module;
#include "detail/prelude.hpp"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/strtod.h>
#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/io/zero_copy_sink.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

export module mcpplibs.mwrap.protobuf.io;

export namespace google::protobuf {
// types (1)
using ::google::protobuf::ZeroCopyCodedInputStream;
}  // namespace google::protobuf

export namespace google::protobuf::io {
// types (26)
using ::google::protobuf::io::AnnotationCollector;
using ::google::protobuf::io::AnnotationProtoCollector;
using ::google::protobuf::io::ArrayInputStream;
using ::google::protobuf::io::ArrayOutputStream;
using ::google::protobuf::io::CodedInputStream;
using ::google::protobuf::io::CodedOutputStream;
using ::google::protobuf::io::ColumnNumber;
using ::google::protobuf::io::ConcatenatingInputStream;
using ::google::protobuf::io::CopyingInputStream;
using ::google::protobuf::io::CopyingInputStreamAdaptor;
using ::google::protobuf::io::CopyingOutputStream;
using ::google::protobuf::io::CopyingOutputStreamAdaptor;
using ::google::protobuf::io::CordInputStream;
using ::google::protobuf::io::CordOutputStream;
using ::google::protobuf::io::EpsCopyOutputStream;
using ::google::protobuf::io::ErrorCollector;
using ::google::protobuf::io::FileInputStream;
using ::google::protobuf::io::FileOutputStream;
using ::google::protobuf::io::IstreamInputStream;
using ::google::protobuf::io::LimitingInputStream;
using ::google::protobuf::io::OstreamOutputStream;
using ::google::protobuf::io::Printer;
using ::google::protobuf::io::StringOutputStream;
using ::google::protobuf::io::Tokenizer;
using ::google::protobuf::io::ZeroCopyInputStream;
using ::google::protobuf::io::ZeroCopyOutputStream;
// functions and operators (6)
using ::google::protobuf::io::NoLocaleStrtod;
using ::google::protobuf::io::SafeDoubleToFloat;
using ::google::protobuf::io::SimpleDtoa;
using ::google::protobuf::io::SimpleFtoa;
using ::google::protobuf::io::as_string_data;
using ::google::protobuf::io::mutable_string_data;
}  // namespace google::protobuf::io
