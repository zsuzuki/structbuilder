//
//
//
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Sample {

struct Test {
public:
  struct Child {
    struct BitField {
      unsigned enabled : 1;
      unsigned index : 5;
      unsigned msg_len : 4;
    };
    BitField field;
    char message[16];
    std::vector<uint16_t> ranking;

    void setMessage(const char *msg, size_t len) {
      if (len > sizeof(message))
        len = sizeof(message);
      strncpy(message, msg, len);
      field.msg_len = len - 1;
      field.enabled = true;
    }
    const char *getMessage() const { return message; }
    size_t getMessageSize() const { return field.msg_len + 1; }
    std::string getMessageString() const {
      return std::string{message, 0, getMessageSize()};
    }
  };

  std::vector<Child> child_list;
};

} // namespace Sample
