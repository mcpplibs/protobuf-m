// A message through a gzip stream and back, with the package's `gzip` feature.
import std;
import mcpplibs.mwrap.protobuf;
import mcpplibs.mwrap.protobuf.io.gzip;

namespace pb = google::protobuf;

int main() {
    pb::Struct object;
    for (int i = 0; i < 200; ++i) (*object.mutable_fields())[std::format("key{}", i)].set_string_value(std::string(32, 'x'));
    const std::string plain = object.SerializeAsString();

    std::string compressed;
    {
        pb::io::StringOutputStream sink(&compressed);
        pb::io::GzipOutputStream::Options options;
        options.format = pb::io::GzipOutputStream::GZIP;
        pb::io::GzipOutputStream gzip(&sink, options);
        if (!object.SerializeToZeroCopyStream(&gzip) || !gzip.Close()) return 1;
    }
    pb::io::ArrayInputStream source(compressed.data(), static_cast<int>(compressed.size()));
    pb::io::GzipInputStream gunzip(&source, pb::io::GzipInputStream::GZIP);
    pb::Struct back;
    const bool ok = back.ParseFromZeroCopyStream(&gunzip) && pb::util::MessageDifferencer::Equals(object, back) &&
                    static_cast<unsigned char>(compressed[0]) == 0x1f && static_cast<unsigned char>(compressed[1]) == 0x8b &&
                    compressed.size() < plain.size();
    std::println("{} bytes, gzip {} bytes, read back {}", plain.size(), compressed.size(), ok ? "equal" : "DIFFERENT");
    return ok ? 0 : 1;
}
