//
//
//
#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include <string>
#include <vector>

//
class Serializer {
private:
  //
  class Exception : public std::exception {
    char error_message[256];

  public:
    Exception(bool wr, std::string msg, size_t p, size_t ds, size_t s) {
      snprintf(error_message, sizeof(error_message),
               "%s%s: pointer=%zu, data size=%zu buffer size=%zu",
               wr ? "[write]" : "[read]", msg.c_str(), p, ds, s);
    }
    Exception(std::string msg, uint16_t need, uint16_t v) {
      snprintf(error_message, sizeof(error_message),
               "[unsupport version] module<%s>: A version greater than %d is "
               "required. However, the data version is %d.",
               msg.c_str(), need, v);
    }
    ~Exception() override = default;
    const char *what() const noexcept override { return error_message; }
  };

  // interface type
  enum class Type : int { Normal, Buffer, Struct, Max };

  void *buffer = nullptr;
  size_t buffer_size = 0;
  size_t w_pointer = 0;
  size_t r_pointer = 0;

  //
  size_t get_read_max() const {
    return (w_pointer == 0 || buffer_size < w_pointer) ? buffer_size
                                                       : w_pointer;
  }
  template <class T> T *get_write_pointer() {
    auto base_ptr = reinterpret_cast<uint8_t *>(buffer) + w_pointer;
    return reinterpret_cast<T *>(base_ptr);
  }
  template <class T> T *get_read_pointer() {
    auto base_ptr = reinterpret_cast<uint8_t *>(buffer) + r_pointer;
    return reinterpret_cast<T *>(base_ptr);
  }
  size_t get_next_pointer(size_t data_size, bool is_write, Type t) const {
    auto max_ptr = is_write ? buffer_size : get_read_max();
    auto base_ptr = is_write ? w_pointer : r_pointer;
    auto next_ptr = base_ptr + data_size;
    if (next_ptr > max_ptr) {
      // buffer over
      static const std::string type_list[static_cast<int>(Type::Max)] = {
          "[number]", "[buffer]", "[struct]"};
      auto msg = type_list[static_cast<int>(t)] + " buffer over!";
      throw Exception(is_write, msg, base_ptr, data_size, max_ptr);
    }
    return next_ptr;
  }

public:
  Serializer() = default;
  ~Serializer() = default;

  using version_t = uint16_t;

  void initialize(void *b, size_t bs) {
    buffer = b;
    buffer_size = bs;
    reset();
  }

  //
  void reset() {
    w_pointer = 0;
    r_pointer = 0;
  }
  size_t getWriteSize() const { return w_pointer; }
  size_t getReadSize() const { return r_pointer; }

  // number
  template <class T = uint32_t> void put(const T &v) {
    auto next_ptr = get_next_pointer(sizeof(T), true, Type::Normal);
    auto ptr = get_write_pointer<T>();
    std::memcpy(ptr, &v, sizeof(T));
    w_pointer = next_ptr;
  }
  //
  template <class T = uint32_t> T get() {
    auto next_ptr = get_next_pointer(sizeof(T), false, Type::Normal);
    auto ptr = get_read_pointer<T>();
    T result;
    std::memcpy(&result, ptr, sizeof(T));
    r_pointer = next_ptr;
    return result;
  }

  // buffer
  template <class T = char, typename TS = uint16_t>
  void putBuffer(const T *source_buffer, TS write_size) {
    auto total_size = sizeof(T) * write_size;
    auto overall_size = total_size + sizeof(TS);
    auto next_ptr = get_next_pointer(overall_size, true, Type::Buffer);
    auto size_ptr = get_write_pointer<TS>();
    std::memcpy(size_ptr, &write_size, sizeof(TS));
    if (write_size > 0) {
      auto buff_ptr = reinterpret_cast<T *>(size_ptr + 1);
      std::memcpy(buff_ptr, source_buffer, total_size);
    }
    w_pointer = next_ptr;
  }
  //
  template <class T = char, typename TS = uint16_t>
  std::pair<const T *, TS> getBuffer() {
    auto next_ptr = get_next_pointer(sizeof(TS), false, Type::Buffer);
    auto size_ptr = get_read_pointer<TS>();
    TS read_size;
    std::memcpy(&read_size, size_ptr, sizeof(TS));
    r_pointer = next_ptr;
    if (read_size > 0) {
      auto total_size = read_size * sizeof(T);
      next_ptr = get_next_pointer(total_size, false, Type::Buffer);
      auto buff_ptr = reinterpret_cast<T *>(size_ptr + 1);
      r_pointer = next_ptr;
      return std::make_pair(buff_ptr, read_size);
    }
    return std::make_pair(nullptr, 0);
  }

  // struct
  template <class T> void putStruct(const T &s) {
    uint16_t ssize = sizeof(T);
    auto overall_size = ssize + sizeof(uint16_t);
    auto next_ptr = get_next_pointer(overall_size, true, Type::Struct);
    auto size_ptr = get_write_pointer<uint16_t>();
    auto buff_ptr = reinterpret_cast<T *>(size_ptr + 1);
    std::memcpy(size_ptr, &ssize, sizeof(uint16_t));
    std::memcpy(buff_ptr, &s, ssize);
    w_pointer = next_ptr;
  }
  //
  template <class T> void getStruct(T &result) {
    auto next_ptr = get_next_pointer(sizeof(uint16_t), false, Type::Struct);
    auto size_ptr = get_read_pointer<uint16_t>();
    uint16_t struct_size;
    std::memcpy(&struct_size, size_ptr, sizeof(uint16_t));
    r_pointer = next_ptr;
    if (struct_size > 0) {
      next_ptr = get_next_pointer(struct_size, false, Type::Struct);
      if (struct_size > sizeof(T))
        struct_size = sizeof(T);
      std::memcpy(&result, reinterpret_cast<T *>(size_ptr + 1), struct_size);
      r_pointer = next_ptr;
    }
  }

  // vector
  template <class T, typename TS = uint16_t>
  void putVector(const std::vector<T> &v) {
    putBuffer<T, TS>(v.data(), v.size());
  }
  //
  template <class T, typename TS = uint16_t> void getVector(std::vector<T> &v) {
    auto r = getBuffer<T, TS>();
    v.resize(r.second);
    std::memcpy(v.data(), r.first, sizeof(T) * r.second);
  }

  // string
  void put(std::string &str) {
    putBuffer<char, uint16_t>(str.data(), str.size());
  }
  void get(std::string &str) {
    auto r = getBuffer<char, uint16_t>();
    str = std::string{r.first, r.second};
  }

  // version
  void putVersion(version_t v) { put(v); }
  version_t getVersion(const char *mod, version_t need) {
    auto v = get<version_t>();
    if (v <= need) {
      throw Exception(mod, need, v);
    }
    return v;
  }
};
