//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
#include "test.hpp"
#include "serializer.hpp"

namespace Sample {
//
size_t getTestPackSize(const Test& s) {
    size_t r = sizeof(uint16_t);
    r += sizeof(uint8_t);
    for (auto& t : s.child_list) {
        r += sizeof(uint16_t) + sizeof(t.field);
        r += sizeof(uint8_t) + sizeof(char) * t.getMessageSize();
        r += sizeof(size_t) + sizeof(uint16_t) * t.ranking.size();
    }
    return r;
}
//
void packTest(Serializer& ser, Test& s) {
    ser.putVersion(1001);
    ser.put<uint8_t>(s.child_list.size());
    for (auto& t : s.child_list) {
        ser.putStruct(t.field);
        ser.putBuffer<char, uint8_t>(t.getMessage(), t.getMessageSize());
        ser.putVector<uint16_t>(t.ranking);
    }
}
//
void unpackTest(Serializer& ser, Test& s) {
    auto version = ser.getVersion("Test", 999);
    auto t_size = ser.get<uint8_t>();
    s.child_list.resize(t_size);
    for (auto& t : s.child_list) {
        ser.getStruct(t.field);
        {
         auto r = ser.getBuffer<char, uint8_t>();
         t.setMessage(r.first, r.second);
        }
        ser.getVector<uint16_t>(t.ranking);
    }
}
} // namespace Sample
