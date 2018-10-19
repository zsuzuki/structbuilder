//
//
//
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Sample {

struct Test {
  struct BitField {
    unsigned enabled : 1;
    unsigned index : 5;
    unsigned msg_len : 4;
  };
  BitField field;
  char message[16];
  std::vector<uint16_t> ranking;

  void setMessageSize(size_t sz) { field.msg_len = sz - 1; }
  size_t getMessageSize() const { return field.msg_len + 1; }
  std::string getMessage() const {
    return std::string{message, 0, getMessageSize()};
  }
};

} // namespace Sample
