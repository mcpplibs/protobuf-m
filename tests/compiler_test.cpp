// Parsing a .proto at run time, through the module alone: the tokenizer over an
// in-memory stream, the parser into a FileDescriptorProto, a DescriptorPool,
// and a message of the parsed type; a syntax error reported with its position.
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.protobuf;

namespace pb = google::protobuf;

namespace {
struct Errors : pb::io::ErrorCollector {
    std::vector<std::string> seen;
    void RecordError(int line, pb::io::ColumnNumber column, std::string_view message) override {
        seen.push_back(std::format("{}:{}: {}", line + 1, column + 1, message));
    }
};

bool parse(std::string_view text, pb::FileDescriptorProto* file, Errors* errors) {
    pb::io::ArrayInputStream input(text.data(), static_cast<int>(text.size()));
    pb::io::Tokenizer tokenizer(&input, errors);
    pb::compiler::Parser parser;
    parser.RecordErrorsTo(errors);
    return parser.Parse(&tokenizer, file);
}
}  // namespace

TEST(Compiler, ParseBuildAndUse) {
    constexpr std::string_view text = R"(
        syntax = "proto3";
        package geo;
        message Point { int32 x = 1; int32 y = 2; }
        message Path { repeated Point points = 1; string label = 2; }
    )";
    pb::FileDescriptorProto file;
    Errors errors;
    ASSERT_TRUE(parse(text, &file, &errors));
    EXPECT_TRUE(errors.seen.empty());
    file.set_name("geo.proto");
    pb::DescriptorPool pool;
    const pb::FileDescriptor* built = pool.BuildFile(file);
    ASSERT_NE(built, nullptr);
    const pb::Descriptor* path = built->FindMessageTypeByName("Path");
    ASSERT_NE(path, nullptr);
    EXPECT_EQ(path->field_count(), 2);
    EXPECT_EQ(path->FindFieldByName("points")->message_type()->full_name(), "geo.Point");

    pb::DynamicMessageFactory factory(&pool);
    std::unique_ptr<pb::Message> message(factory.GetPrototype(path)->New());
    ASSERT_TRUE(pb::TextFormat::ParseFromString("label: \"route\" points { x: 1 y: 2 } points { x: -3 }", message.get()));
    std::string json;
    ASSERT_TRUE(pb::json::MessageToJsonString(*message, &json).ok());
    EXPECT_EQ(json, R"({"points":[{"x":1,"y":2},{"x":-3}],"label":"route"})");
}

TEST(Compiler, SyntaxErrorIsReported) {
    pb::FileDescriptorProto file;
    Errors errors;
    EXPECT_FALSE(parse("syntax = \"proto3\";\nmessage Broken { int32 = 1; }\n", &file, &errors));
    ASSERT_FALSE(errors.seen.empty());
    EXPECT_TRUE(errors.seen.front().starts_with("2:")) << errors.seen.front();
}
