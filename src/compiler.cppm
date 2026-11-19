// Written by tools/surface.py from tools/surface.json; tools/check_exports.py audits it. Do not edit by hand.

// mcpplibs.mwrap.protobuf.compiler — parsing .proto files at run time: Importer, DiskSourceTree, Parser (protobuf 35.1), unchanged:
// each name is a using-declaration of the upstream entity in the namespace upstream declares it in.
module;
#include "detail/prelude.hpp"
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/compiler/parser.h>

export module mcpplibs.mwrap.protobuf.compiler;

export namespace google::protobuf::compiler {
// types (7)
using ::google::protobuf::compiler::DiskSourceTree;
using ::google::protobuf::compiler::Importer;
using ::google::protobuf::compiler::MultiFileErrorCollector;
using ::google::protobuf::compiler::Parser;
using ::google::protobuf::compiler::SourceLocationTable;
using ::google::protobuf::compiler::SourceTree;
using ::google::protobuf::compiler::SourceTreeDescriptorDatabase;
}  // namespace google::protobuf::compiler
