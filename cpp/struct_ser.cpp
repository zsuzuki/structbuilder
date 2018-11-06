//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//

#include "struct.hpp"

namespace Sample {
namespace {
} // namespace

//
size_t Test::getSerializeSize() const {
    size_t r = sizeof(Serializer::version_t);
    r += sizeof(uint16_t) + sizeof(bit_field);
    r += sizeof(count);
    r += sizeof(max_speed);
    r += sizeof(uint16_t) + sizeof(uint8_t) * ranking.size();
    r += sizeof(uint16_t) + sizeof(float) * line.size();
    r += sizeof(uint16_t);
    for (auto& tNote : note) {
    r += sizeof(tNote.page);
    r += sizeof(tNote.line);
    }
    r += sizeof(uint16_t) + sizeof(child.bit_field);
    r += sizeof(uint16_t) + sizeof(char) * child.name.size();
    r += sizeof(uint16_t);
    for (auto& tEntry : entry_list) {
    r += sizeof(uint16_t) + sizeof(char) * tEntry.name.size();
    r += sizeof(uint16_t) + sizeof(char) * tEntry.country.size();
    r += sizeof(tEntry.point);
    r += sizeof(tEntry.wins);
    }
    return r;
}
//
void Test::serialize(Serializer& ser) {
    ser.putVersion(100);
    ser.putStruct(bit_field);
    ser.put<int>(count);
    ser.put<uint32_t>(max_speed);
    ser.putVector<uint8_t>(ranking);
    ser.putVector<float>(line);
    ser.put<uint16_t>(note.size());
    for (auto& tNote : note) {
    ser.put<int>(tNote.page);
    ser.put<int>(tNote.line);
    }
    ser.putStruct(child.bit_field);
    ser.put(child.name);
    ser.put<uint16_t>(entry_list.size());
    for (auto& tEntry : entry_list) {
    ser.put(tEntry.name);
    ser.put(tEntry.country);
    ser.put<uint16_t>(tEntry.point);
    ser.put<uint8_t>(tEntry.wins);
    }
}
//
void Test::deserialize(Serializer& ser) {
    auto version = ser.getVersion("Test", 100);
    (void)version;
    ser.getStruct(bit_field);
    count = ser.get<int>();
    max_speed = ser.get<uint32_t>();
    ser.getVector<uint8_t>(ranking);
    ser.getVector<float>(line);
    auto tNoteSize = ser.get<uint16_t>();
    for (size_t cNote = 0; cNote < tNoteSize; cNote++) {
        Note tNote{};
    tNote.page = ser.get<int>();
    tNote.line = ser.get<int>();
    if (cNote < 4)
        note[cNote] = tNote;
    }
    ser.getStruct(child.bit_field);
    ser.get(child.name);
    entry_list.resize(ser.get<uint16_t>());
    for (auto& tEntry : entry_list) {
    ser.get(tEntry.name);
    ser.get(tEntry.country);
    tEntry.point = ser.get<uint16_t>();
    tEntry.wins = ser.get<uint8_t>();
    }
}
} // namespace Sample
