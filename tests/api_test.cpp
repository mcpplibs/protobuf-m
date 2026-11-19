// The API through the module alone: no protobuf header in this translation
// unit. A message type built at run time (FileDescriptorProto -> DescriptorPool
// -> DynamicMessage) with its wire bytes pinned; text and JSON round trips;
// the well-known types; MessageDifferencer, FieldMaskUtil, delimited streams,
// coded streams and an arena.
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.protobuf;

namespace pb = google::protobuf;

namespace {
std::string hex(std::string_view bytes) {
    std::string out;
    for (unsigned char c : bytes) out += std::format("{:02x}", c);
    return out;
}

// demo.Person { string name = 1; int32 id = 2; repeated string email = 3; }
const pb::FileDescriptor* person_file(pb::DescriptorPool& pool) {
    pb::FileDescriptorProto file;
    file.set_name("person.proto");
    file.set_package("demo");
    file.set_syntax("proto3");
    pb::DescriptorProto* message = file.add_message_type();
    message->set_name("Person");
    auto field = [&](const char* name, int number, pb::FieldDescriptorProto::Type type, pb::FieldDescriptorProto::Label label) {
        pb::FieldDescriptorProto* f = message->add_field();
        f->set_name(name);
        f->set_json_name(name);
        f->set_number(number);
        f->set_type(type);
        f->set_label(label);
    };
    field("name", 1, pb::FieldDescriptorProto::TYPE_STRING, pb::FieldDescriptorProto::LABEL_OPTIONAL);
    field("id", 2, pb::FieldDescriptorProto::TYPE_INT32, pb::FieldDescriptorProto::LABEL_OPTIONAL);
    field("email", 3, pb::FieldDescriptorProto::TYPE_STRING, pb::FieldDescriptorProto::LABEL_REPEATED);
    return pool.BuildFile(file);
}

struct Person {
    pb::DescriptorPool pool;
    const pb::Descriptor* type = nullptr;
    pb::DynamicMessageFactory factory{&pool};
    Person() { type = person_file(pool)->FindMessageTypeByName("Person"); }
    std::unique_ptr<pb::Message> make(std::string_view name, int id, std::initializer_list<std::string_view> emails) {
        std::unique_ptr<pb::Message> m(factory.GetPrototype(type)->New());
        const pb::Reflection* r = m->GetReflection();
        r->SetString(m.get(), type->FindFieldByName("name"), std::string(name));
        r->SetInt32(m.get(), type->FindFieldByName("id"), id);
        for (auto e : emails) r->AddString(m.get(), type->FindFieldByName("email"), std::string(e));
        return m;
    }
};
}  // namespace

TEST(Api, Version) {
    EXPECT_EQ(mcpplibs::mwrap::protobuf::macros::GOOGLE_PROTOBUF_VERSION, 7035001);
    EXPECT_EQ(mcpplibs::mwrap::protobuf::macros::PROTOBUF_VERSION, 7035001);
}

TEST(Api, DynamicMessageWireBytes) {
    Person person;
    ASSERT_NE(person.type, nullptr);
    EXPECT_EQ(person.type->full_name(), "demo.Person");
    auto ada = person.make("Ada", 7, {"ada@example.org"});
    std::string wire;
    ASSERT_TRUE(ada->SerializeToString(&wire));
    // Field 1 (len 3) "Ada", field 2 varint 7, field 3 (len 15) "ada@example.org".
    EXPECT_EQ(hex(wire), "0a034164611007" "1a0f616461406578616d706c652e6f7267");
    std::unique_ptr<pb::Message> back(ada->New());
    ASSERT_TRUE(back->ParseFromString(wire));
    EXPECT_TRUE(pb::util::MessageDifferencer::Equals(*ada, *back));
    EXPECT_EQ(back->ByteSizeLong(), wire.size());
}

TEST(Api, TextFormat) {
    Person person;
    auto ada = person.make("Ada", 7, {"a@x", "b@x"});
    std::string text;
    ASSERT_TRUE(pb::TextFormat::PrintToString(*ada, &text));
    EXPECT_EQ(text, "name: \"Ada\"\nid: 7\nemail: \"a@x\"\nemail: \"b@x\"\n");
    std::unique_ptr<pb::Message> back(ada->New());
    ASSERT_TRUE(pb::TextFormat::ParseFromString("name: \"Ada\" id: 7 email: \"a@x\" email: \"b@x\"", back.get()));
    EXPECT_TRUE(pb::util::MessageDifferencer::Equals(*ada, *back));
    EXPECT_FALSE(pb::TextFormat::ParseFromString("name: 12 id: \"x\"", back.get()));
}

TEST(Api, Json) {
    Person person;
    auto ada = person.make("Ada", 7, {"ada@example.org"});
    std::string json;
    ASSERT_TRUE(pb::json::MessageToJsonString(*ada, &json).ok());
    EXPECT_EQ(json, R"({"name":"Ada","id":7,"email":["ada@example.org"]})");
    std::unique_ptr<pb::Message> back(ada->New());
    ASSERT_TRUE(pb::util::JsonStringToMessage(json, back.get()).ok());
    EXPECT_TRUE(pb::util::MessageDifferencer::Equals(*ada, *back));
    const auto bad = pb::util::JsonStringToMessage(R"({"id":"seven"})", back.get());
    EXPECT_FALSE(bad.ok());
    EXPECT_FALSE(bad.message().empty());
}

TEST(Api, WellKnownTypes) {
    pb::Timestamp when;
    ASSERT_TRUE(pb::util::TimeUtil::FromString("2026-11-19T08:30:00Z", &when));
    EXPECT_EQ(when.seconds(), 1795077000);
    const pb::Duration ninety = pb::util::TimeUtil::SecondsToDuration(90);
    EXPECT_EQ(pb::util::TimeUtil::ToString(when + ninety), "2026-11-19T08:31:30Z");
    EXPECT_EQ(pb::util::TimeUtil::ToString(ninety * 2), "180s");
    EXPECT_TRUE(when < when + ninety);

    pb::Struct object;
    (*object.mutable_fields())["pi"].set_number_value(3.25);
    (*object.mutable_fields())["ok"].set_bool_value(true);
    (*object.mutable_fields())["tags"].mutable_list_value()->add_values()->set_string_value("x");
    std::string json;
    ASSERT_TRUE(pb::json::MessageToJsonString(object, &json).ok());
    pb::Struct back;
    ASSERT_TRUE(pb::json::JsonStringToMessage(json, &back).ok());
    EXPECT_TRUE(pb::util::MessageDifferencer::Equals(object, back));
    EXPECT_EQ(back.fields().at("pi").number_value(), 3.25);

    pb::Any any;
    ASSERT_TRUE(any.PackFrom(when));
    EXPECT_EQ(any.type_url(), "type.googleapis.com/google.protobuf.Timestamp");
    EXPECT_TRUE(any.Is<pb::Timestamp>());
    EXPECT_FALSE(any.Is<pb::Duration>());
    pb::Timestamp unpacked;
    ASSERT_TRUE(any.UnpackTo(&unpacked));
    EXPECT_EQ(unpacked.seconds(), when.seconds());

    pb::Int64Value wrapped;
    wrapped.set_value(-5);
    EXPECT_EQ(wrapped.SerializeAsString(), std::string("\x08\xfb\xff\xff\xff\xff\xff\xff\xff\xff\x01", 11));
}

TEST(Api, FieldMaskAndDifferencer) {
    pb::FieldMask mask;
    pb::util::FieldMaskUtil::FromString("seconds,nanos", &mask);
    EXPECT_EQ(mask.paths_size(), 2);
    EXPECT_TRUE(pb::util::FieldMaskUtil::IsValidFieldMask<pb::Timestamp>(mask));
    pb::Timestamp a, b;
    a.set_seconds(1);
    b.set_seconds(2);
    pb::util::MessageDifferencer differ;
    std::string report;
    differ.ReportDifferencesToString(&report);
    EXPECT_FALSE(differ.Compare(a, b));
    EXPECT_NE(report.find("seconds"), std::string::npos) << report;
}

TEST(Api, CodedAndDelimitedStreams) {
    std::string bytes;
    {
        pb::io::StringOutputStream raw(&bytes);
        pb::io::CodedOutputStream coded(&raw);
        coded.WriteVarint32(300);
        coded.WriteLittleEndian32(0x01020304u);
    }
    EXPECT_EQ(hex(bytes), "ac0204030201");
    pb::io::ArrayInputStream in(bytes.data(), static_cast<int>(bytes.size()));
    pb::io::CodedInputStream reader(&in);
    std::uint32_t varint = 0, fixed = 0;
    ASSERT_TRUE(reader.ReadVarint32(&varint));
    ASSERT_TRUE(reader.ReadLittleEndian32(&fixed));
    EXPECT_EQ(varint, 300u);
    EXPECT_EQ(fixed, 0x01020304u);

    std::string stream;
    {
        pb::io::StringOutputStream out(&stream);
        for (int seconds : {1, 2, 3}) {
            pb::Timestamp t;
            t.set_seconds(seconds);
            ASSERT_TRUE(pb::util::SerializeDelimitedToZeroCopyStream(t, &out));
        }
    }
    pb::io::ArrayInputStream source(stream.data(), static_cast<int>(stream.size()));
    for (int seconds : {1, 2, 3}) {
        pb::Timestamp t;
        bool clean_eof = false;
        ASSERT_TRUE(pb::util::ParseDelimitedFromZeroCopyStream(&t, &source, &clean_eof));
        EXPECT_EQ(t.seconds(), seconds);
    }
}

TEST(Api, Arena) {
    pb::Arena arena;
    auto* when = pb::Arena::Create<pb::Timestamp>(&arena);
    when->set_seconds(42);
    EXPECT_EQ(when->GetArena(), &arena);
    EXPECT_GT(arena.SpaceUsed(), 0u);
}
