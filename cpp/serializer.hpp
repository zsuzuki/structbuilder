//
//
//
#pragma once

#include <cstdint>
#include <exception>
#include <string>
#include <cstring>

//
class Serializer {
private:
    void* buffer = nullptr;
    size_t buffer_size  = 0;
    size_t w_pointer = 0;
    size_t r_pointer = 0;

    //    
    size_t get_read_max() const {
        return (w_pointer == 0 || buffer_size < w_pointer) ? buffer_size : w_pointer;
    }
    
    //
    class Exception : public std::exception {
        char error_message[256];
    public:
        Exception(bool wr, const char* msg, size_t p, size_t ds, size_t s)
        {
            snprintf(error_message,sizeof(error_message),"%s%s: pointer=%zu, data size=%zu buffer size=%zu",
                     wr ? "[write]" : "[read]",
                     msg,
                     p,
                     ds,
                     s);
        }
        ~Exception() override = default;
        const char* what() const noexcept override { return error_message; }
    };
public:
    Serializer() = default;
    ~Serializer() = default;

    void initialize(void* b, size_t bs) { buffer = b; buffer_size = bs; reset(); }

    void reset() { w_pointer = 0; r_pointer = 0; }
    size_t getWriteSize() const { return w_pointer; }
    size_t getReadSize() const { return r_pointer; }

    // number
    template <class T=uint32_t>
    void put(const T& v)
    {
        auto next_ptr = w_pointer + sizeof(T);
        if (next_ptr > buffer_size)
        {
            // buffer over
            throw Exception(true, "[number] buffer over!", w_pointer, sizeof(T), buffer_size);
        }
        auto base_ptr = reinterpret_cast<uint8_t*>(buffer) + w_pointer;
        auto ptr = reinterpret_cast<T*>(base_ptr);
        std::memcpy(ptr, &v, sizeof(T));
        w_pointer = next_ptr;
    }
    //
    template <class T=uint32_t>
    T get()
    {
        auto next_ptr = r_pointer + sizeof(T);
        auto max_pointer = get_read_max();
        if (next_ptr > max_pointer)
        {
            // buffer over
            throw Exception(false, "[number] buffer over!", r_pointer, sizeof(T), max_pointer);
        }
        auto base_ptr = reinterpret_cast<uint8_t*>(buffer) + r_pointer;
        auto ptr = reinterpret_cast<T*>(base_ptr);
        T result;
        std::memcpy(&result, ptr, sizeof(T));
        r_pointer = next_ptr;
        return result;
    }
    // buffer
    template <class T=char*,typename TS=size_t>
    void putBuffer(T source_buffer, TS write_size)
    {
        auto next_ptr = w_pointer + write_size + sizeof(TS);
        if (next_ptr > buffer_size)
        {
            // buffer over
            throw Exception(true, "[buffer] buffer over!", w_pointer, write_size + sizeof(TS), buffer_size);
        }
        auto base_ptr = reinterpret_cast<uint8_t*>(buffer) + w_pointer;
        auto size_ptr = reinterpret_cast<TS*>(base_ptr);
        auto buff_ptr = reinterpret_cast<T>(size_ptr + 1);
        std::memcpy(size_ptr, &write_size, sizeof(TS));
        std::memcpy(buff_ptr, source_buffer, write_size);
        w_pointer = next_ptr;
    }
    //
    template <class T=char*,typename TS=size_t>
    std::pair<T,TS> getBuffer()
    {
        auto next_ptr = r_pointer + sizeof(TS);
        auto max_pointer = get_read_max();
        if (next_ptr > max_pointer)
        {
            // buffer over
            throw Exception(false, "[buffer] buffer over!", r_pointer, sizeof(TS), max_pointer);
        }
        auto base_ptr = reinterpret_cast<uint8_t*>(buffer) + r_pointer;
        auto size_ptr = reinterpret_cast<TS*>(base_ptr);
        TS read_size;
        std::memcpy(&read_size, size_ptr,sizeof(TS));
        if (read_size > 0)
        {
            next_ptr += read_size;
            if (next_ptr > max_pointer)
            {
                // buffer over
                throw Exception(false, "[buffer] buffer over!", r_pointer, read_size, max_pointer);
            }
            auto buff_ptr = reinterpret_cast<T>(size_ptr + 1);
            r_pointer = next_ptr;
            return std::make_pair(buff_ptr, read_size);
        }
        return std::make_pair(nullptr,0);
    }
};
