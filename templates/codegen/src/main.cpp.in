// Generated code with the module: addressbook.pb.h is produced from
// proto/addressbook.proto by protoc during the build. A translation unit that
// includes generated headers includes the prelude first (every header the
// modules include), then the generated ones, then imports (GCC needs every
// #include before the first import).
#include <mwrap/protobuf/prelude.hpp>
#include "addressbook.pb.h"

import std;
import mcpplibs.mwrap.protobuf;

namespace gpb = google::protobuf;  // (generated code declares its own `pb`)

int main() {
    tutorial::AddressBook book;
    tutorial::Person* ada = book.add_people();
    ada->set_name("Ada Lovelace");
    ada->set_id(1815);
    ada->set_email("ada@example.org");
    tutorial::Person::PhoneNumber* phone = ada->add_phones();
    phone->set_number("+44 20 0000 0000");
    phone->set_type(tutorial::Person::PHONE_TYPE_HOME);
    gpb::util::TimeUtil::FromString("1843-07-01T00:00:00Z", ada->mutable_last_updated());

    const std::string wire = book.SerializeAsString();
    std::string json;
    (void)gpb::json::MessageToJsonString(book, &json);
    std::println("{} bytes on the wire\n{}", wire.size(), json);

    tutorial::AddressBook back;
    const bool ok = back.ParseFromString(wire) && gpb::util::MessageDifferencer::Equals(book, back) &&
                    back.people(0).phones(0).type() == tutorial::Person::PHONE_TYPE_HOME &&
                    tutorial::Person::descriptor()->full_name() == "tutorial.Person";
    std::println("{}", ok ? "read back: equal" : "read back: DIFFERENT");
    return ok ? 0 : 1;
}
