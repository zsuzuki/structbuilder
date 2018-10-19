//
//
//

#include "test.hpp"
#include "serializer.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {
struct TestBlock {
  std::vector<Sample::Test> test_list;
};
//
size_t getTestSize(Sample::Test &t) {
  auto calc_vec = [](auto &v) {
    size_t r = 0;
    for (auto &e : v) {
      r += sizeof(e);
    }
    return r;
  };
  return sizeof(uint16_t) + sizeof(t.field) + sizeof(uint8_t) +
         t.getMessageSize() + sizeof(size_t) + calc_vec(t.ranking);
}
//
size_t getTotalTestSize(TestBlock &b) {
  auto test_size = sizeof(uint16_t) + sizeof(uint32_t);
  // test
  test_size += sizeof(uint8_t);
  for (auto &t : b.test_list) {
    test_size += getTestSize(t);
  }
  return test_size;
}
//
void packTest(Serializer &ser, TestBlock &b) {
  ser.put<uint32_t>(getTotalTestSize(b)); // size
  ser.put<uint16_t>(1000);                // version
  ser.put<uint8_t>(b.test_list.size());
  for (auto &t : b.test_list) {
    // type: struct
    ser.putStruct(t.field);
    // type: attribute/method
    ser.putBuffer<char, uint8_t>(t.message, t.getMessageSize());
    // type: container
    ser.putVector<uint16_t>(t.ranking);
  }
  std::cout << "serialize size: " << ser.getWriteSize() << std::endl;
}
//
void unpackTest(Serializer &ser, TestBlock &b) {
  auto total_size = ser.get<uint32_t>();
  auto version = ser.get<uint16_t>();
  auto nb_test = ser.get<uint8_t>();
  std::cout << "deserialize: version=" << version
            << " total size=" << total_size
            << " number of structs=" << (int)nb_test << std::endl;
  b.test_list.resize(nb_test);
  for (auto &t : b.test_list) {
    // type: struct
    ser.getStruct(t.field);
    // type: attribute/method
    auto t_message = ser.getBuffer<char, uint8_t>();
    if (t_message.second)
      memcpy(t.message, t_message.first, t_message.second);
    t.setMessageSize(t_message.second);
    // type: container
    ser.getVector<uint16_t>(t.ranking);
  }
}
} // namespace

//
//
//
int main(int argc, const char **argv) {
  if (argc < 2) {
    std::cerr << "no arguments" << std::endl;
    return 1;
  }
  TestBlock block;
  std::vector<Sample::Test> &tlist = block.test_list;
  // data fill
  for (int i = 1; i < argc; i++) {
    auto t = Sample::Test{};
    auto &f = t.field;
    auto msg_len = strlen(argv[i]);
    if (msg_len > sizeof(t.message))
      msg_len = sizeof(t.message);
    f.enabled = msg_len > 0;
    f.index = i - 1;
    t.setMessageSize(msg_len);
    for (int j = 0; j < i; j++)
      t.ranking.emplace_back(j == f.index ? 100 : j * i);
    std::memcpy(t.message, argv[i], msg_len);
    tlist.emplace_back(t);
  }

  // serialize
  Serializer ser{};
  uint8_t ser_buffer[256];
  ser.initialize(ser_buffer, sizeof(ser_buffer));
  packTest(ser, block);

  // deserialize
  TestBlock read_block;
  unpackTest(ser, read_block);
  for (auto &t : read_block.test_list) {
    if (t.field.enabled) {
      std::cout << "[" << t.field.index << "," << t.getMessageSize()
                << "]:" << t.getMessage() << ":";
      for (auto n : t.ranking)
        std::cout << " " << n << ",";
      std::cout << std::endl;
    } else
      std::cout << "DISABLED" << std::endl;
  }
  return 0;
}
