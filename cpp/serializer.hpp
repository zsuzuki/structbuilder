//
//
//
#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include <string>

//
class Serializer {
private:
  void *buffer = nullptr;
  size_t buffer_size = 0;
  size_t w_pointer = 0;
  size_t r_pointer = 0;

  //
  size_t get_read_max() const {
    return (w_pointer == 0 || buffer_size < w_pointer) ? buffer_size
                                                       : w_pointer;
  }

  //
  class Exception : public std::exception {
    char error_message[256];

  public:
    Exception(bool wr, const char *msg, size_t p, size_t ds, size_t s) {
      snprintf(error_message, sizeof(error_message),
               "%s%s: pointer=%zu, data size=%zu buffer size=%zu",
               wr ? "[write]" : "[read]", msg, p, ds, s);
    }
    ~Exception() override = default;
    const char *what() const noexcept override { return error_message; }
  };

public:
  Serializer() = default;
  ~Serializer() = default;

  void initialize(void *b, size_t bs) {
    buffer = b;
    buffer_size = bs;
    reset();
  }

  void reset() {
    w_pointer = 0;
    r_pointer = 0;
  }
  size_t getWriteSize() const { return w_pointer; }
  size_t getReadSize() const { return r_pointer; }

  // number
  template <class T = uint32_t> void put(const T &v) {
    auto next_ptr = w_pointer + sizeof(T);
    if (next_ptr > buffer_size) {
      // buffer over
      throw Exception(true, "[number] buffer over!", w_pointer, sizeof(T),
                      buffer_size);
    }
    auto base_ptr = reinterpret_cast<uint8_t *>(buffer) + w_pointer;
    auto ptr = reinterpret_cast<T *>(base_ptr);
    std::memcpy(ptr, &v, sizeof(T));
    w_pointer = next_ptr;
  }
  //
  template <class T = uint32_t> T get() {
    auto next_ptr = r_pointer + sizeof(T);
    auto max_pointer = get_read_max();
    if (next_ptr > max_pointer) {
      // buffer over
      throw Exception(false, "[number] buffer over!", r_pointer, sizeof(T),
                      max_pointer);
    }
    auto base_ptr = reinterpret_cast<uint8_t *>(buffer) + r_pointer;
    auto ptr = reinterpret_cast<T *>(base_ptr);
    T result;
    std::memcpy(&result, ptr, sizeof(T));
    r_pointer = next_ptr;
    return result;
  }
  // buffer
  template <class T = char, typename TS = size_t>
  void putBuffer(const T *source_buffer, TS write_size) {
    auto total_size = sizeof(T) * write_size;
    auto next_ptr = w_pointer + total_size + sizeof(TS);
    if (next_ptr > buffer_size) {
      // buffer over
      throw Exception(true, "[buffer] buffer over!", w_pointer,
                      total_size + sizeof(TS), buffer_size);
    }
    auto base_ptr = reinterpret_cast<uint8_t *>(buffer) + w_pointer;
    auto size_ptr = reinterpret_cast<TS *>(base_ptr);
    auto buff_ptr = reinterpret_cast<T *>(size_ptr + 1);
    std::memcpy(size_ptr, &write_size, sizeof(TS));
    std::memcpy(buff_ptr, source_buffer, total_size);
    w_pointer = next_ptr;
  }
  //
  template <class T = char, typename TS = size_t>
  std::pair<const T *, TS> getBuffer() {
    auto next_ptr = r_pointer + sizeof(TS);
    auto max_pointer = get_read_max();
    if (next_ptr > max_pointer) {
      // buffer over
      throw Exception(false, "[buffer] buffer over!", r_pointer, sizeof(TS),
                      max_pointer);
    }
    auto base_ptr = reinterpret_cast<uint8_t *>(buffer) + r_pointer;
    auto size_ptr = reinterpret_cast<TS *>(base_ptr);
    TS read_size;
    std::memcpy(&read_size, size_ptr, sizeof(TS));
    if (read_size > 0) {
      auto total_size = read_size * sizeof(T);
      next_ptr += total_size;
      if (next_ptr > max_pointer) {
        // buffer over
        throw Exception(false, "[buffer] buffer over!", r_pointer + sizeof(TS),
                        total_size, max_pointer);
      }
      auto buff_ptr = reinterpret_cast<T *>(size_ptr + 1);
      r_pointer = next_ptr;
      return std::make_pair(buff_ptr, read_size);
    }
    return std::make_pair(nullptr, 0);
  }
  // struct
  template <class T> void putStruct(const T &s) {
    uint16_t ssize = sizeof(T);
    auto next_ptr = w_pointer + ssize + sizeof(uint16_t);
    if (next_ptr > buffer_size) {
      // buffer over
      throw Exception(true, "[struct] buffer over!", w_pointer,
                      ssize + sizeof(uint16_t), buffer_size);
    }
    auto base_ptr = reinterpret_cast<uint8_t *>(buffer) + w_pointer;
    auto size_ptr = reinterpret_cast<uint16_t *>(base_ptr);
    auto buff_ptr = reinterpret_cast<T *>(size_ptr + 1);
    std::memcpy(size_ptr, &ssize, sizeof(uint16_t));
    std::memcpy(buff_ptr, &s, ssize);
    w_pointer = next_ptr;
  }
  //
  template <class T> void getStruct(T &result) {
    auto next_ptr = r_pointer + sizeof(uint16_t);
    auto max_pointer = get_read_max();
    if (next_ptr > max_pointer) {
      // buffer over
      throw Exception(false, "[struct] buffer over!", r_pointer,
                      sizeof(uint16_t), max_pointer);
    }
    auto base_ptr = reinterpret_cast<uint8_t *>(buffer) + r_pointer;
    auto size_ptr = reinterpret_cast<uint16_t *>(base_ptr);
    uint16_t struct_size;
    std::memcpy(&struct_size, size_ptr, sizeof(uint16_t));
    if (struct_size > 0) {
      next_ptr += struct_size;
      if (next_ptr > max_pointer) {
        // buffer over
        throw Exception(false, "[struct] buffer over!",
                        r_pointer + sizeof(uint16_t), struct_size, max_pointer);
      }
      if (struct_size > sizeof(T))
        struct_size = sizeof(T);
      memcpy(&result, reinterpret_cast<T *>(size_ptr + 1), struct_size);
      r_pointer = next_ptr;
    }
  }
  // vector
  template <class T, typename TS = size_t>
  void putVector(const std::vector<T> &v) {
    putBuffer<T, TS>(v.data(), v.size());
  }
  //
  template <class T, typename TS = size_t> void getVector(std::vector<T> &v) {
    auto r = getBuffer<T, TS>();
    v.resize(r.second);
    memcpy(v.data(), r.first, sizeof(T) * r.second);
  }
};
