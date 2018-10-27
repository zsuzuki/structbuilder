//
//
//

#include "test.hpp"
#include "serializer.hpp"
#include "test_ser.hpp"

#include <iostream>
#include <string>
#include <vector>

//
//
//
int main(int argc, const char **argv) {
  if (argc < 2) {
    std::cerr << "no arguments" << std::endl;
    return 1;
  }
  Sample::Test test;
  auto &children = test.child_list;
  // data fill
  for (int i = 1; i < argc; i++) {
    auto c = Sample::Test::Child{};
    auto &f = c.field;
    f.index = i - 1;
    int msg_len = std::strlen(argv[i]);
    for (int j = 0; j < msg_len; j++)
      c.ranking.emplace_back(j == f.index ? 100 : j * i);
    c.setMessage(argv[i], msg_len);
    children.emplace_back(c);
  }

  // serialize
  Serializer ser{};
  uint8_t ser_buffer[256];
  ser.initialize(ser_buffer, sizeof(ser_buffer));
  try {
    std::cout << "target serialize size: " << Sample::getTestPackSize(test)
              << std::endl;
    packTest(ser, test);
    std::cout << "serialize size: " << ser.getWriteSize() << std::endl;

    // deserialize
    Sample::Test read_test;
    unpackTest(ser, read_test);
    for (auto &t : read_test.child_list) {
      if (t.field.enabled) {
        std::cout << "[" << t.field.index << "," << t.getMessageSize()
                  << "]:" << t.getMessageString() << ":";
        for (auto n : t.ranking)
          std::cout << " " << n << ",";
        std::cout << std::endl;
      } else
        std::cout << "DISABLED" << std::endl;
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
