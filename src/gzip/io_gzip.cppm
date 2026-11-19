// Written by tools/surface.py from tools/surface.json; tools/check_exports.py audits it. Do not edit by hand.

// mcpplibs.mwrap.protobuf.io.gzip — GzipInputStream and GzipOutputStream (the package's `gzip` feature) (protobuf 35.1), unchanged:
// each name is a using-declaration of the upstream entity in the namespace upstream declares it in.
module;
#include "../detail/prelude.hpp"
#include <google/protobuf/io/gzip_stream.h>
#include "../detail/epilogue.hpp"

export module mcpplibs.mwrap.protobuf.io.gzip;

export namespace google::protobuf::io {
// types (2)
using ::google::protobuf::io::GzipInputStream;
using ::google::protobuf::io::GzipOutputStream;
}  // namespace google::protobuf::io
