#include "serializer.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  try {
    std::cout << "Start" << std::endl;
    auto ser = Serializer{};
    char buffer[128];
    ser.initialize(buffer, sizeof(buffer));

    //
    // number test
    //
    static constexpr int nb_num = 32;
    for (int i = 0; i < nb_num; i++) {
      ser.put<int>(i * 2);
    }
    std::cout << "put number done: " << ser.getWriteSize() << " bytes"
              << std::endl;
    for (int i = 0; i < nb_num; i++) {
      std::cout << " " << ser.get<int>();
    }
    std::cout << "\nget number done: " << ser.getReadSize() << " bytes"
              << std::endl;

    //
    // string test
    //
    ser.reset();
    std::vector<const char *> str = {
        "Hello, World", "Serializer is serializer", "Buffer type <char>",
        "serialize test program", "message of contents"};
    using msg_size_t = uint8_t;
    for (auto s : str) {
      auto sz = strlen(s) + 1;
      ser.putBuffer<char, msg_size_t>(s, sz);
    }
    std::cout << "put string done: " << ser.getWriteSize() << " bytes"
              << std::endl;
    for (size_t i = 0; i < str.size(); i++) {
      auto r = ser.getBuffer<const char, msg_size_t>();
      std::cout << (int)r.second << ": " << r.first << std::endl;
    }
    std::cout << "get string done: " << ser.getReadSize() << " bytes"
              << std::endl;
    ser.reset();
    std::string msg1 = "String Method", msg2 = "Serializer";
    std::cout << "MSG1: " << msg1 << ", MSG2: " << msg2
              << ", size: " << msg1.size() << std::endl;
    ser.put(msg1);
    ser.get(msg2);
    std::cout << "MSG1: " << msg1 << ", MSG2: " << msg2
              << ", size: " << ser.getWriteSize() << std::endl;

    //
    // bit field
    // NOTE: ** not aligned for test **
    //
    struct Field {
      unsigned enabled : 1;
      unsigned index : 5;
      unsigned message : 10;
      unsigned : 14;
    };
    ser.reset();
    static constexpr msg_size_t nb_f = 5;
    for (int i = 0; i < nb_f; i++) {
      Field f{};
      f.enabled = i % 3 == 0;
      f.index = i;
      f.message = 10000 - i * 144;
      ser.put<Field>(f);
    }
    std::cout << "put bit-field struct done: " << ser.getWriteSize() << " bytes"
              << std::endl;
    for (int i = 0; i < nb_f; i++) {
      auto f = ser.get<Field>();
      std::cout << "F: enabled=" << f.enabled << " index=" << f.index
                << " message=" << f.message << std::endl;
    }
    std::cout << "get bit-field struct done: " << ser.getReadSize() << " bytes"
              << std::endl;
  } catch (std::exception &e) {
    std::cerr << "\n*** " << e.what() << " ***" << std::endl;
  }
  return 0;
}