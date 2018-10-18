#include "serializer.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

int
main(int argc,char** argv)
{
    try {
        std::cout << "Start" << std::endl;
        auto ser = Serializer{};
        char buffer[128];
        ser.initialize(buffer, sizeof(buffer));
        
        // number test
        static constexpr int nb_num = 32;
        for (int i = 0; i < nb_num; i++)
        {
            ser.put<int>(i * 2);
        }
        std::cout << "put number done: " << ser.getWriteSize() << " bytes" << std::endl;
        for (int i = 0; i < nb_num; i++)
        {
            std::cout << " " << ser.get<int>();
        }
        std::cout << "\nget number done: " << ser.getReadSize() << " bytes" << std::endl;

        // string test
        ser.reset();
        std::vector<const char*> str = {
            "Hello, World",
            "Serializer is serializer",
            "Buffer type <char>",
            "serialize test program",
            "message of contents"
        };
        using msg_size_t = uint8_t;
        ser.put<msg_size_t>(str.size());
        for (auto s : str)
        {
            auto sz = strlen(s) + 1;
            ser.putBuffer<char*,msg_size_t>(const_cast<char*>(s), sz);
        }
        std::cout << "put string done: " << ser.getWriteSize() << " bytes" << std::endl;
        auto nb_msg = ser.get<msg_size_t>();
        for (int i = 0; i < nb_msg; i++)
        {
            auto r = ser.getBuffer<const char*, msg_size_t>();
            std::cout << (int)r.second << ": " << r.first << std::endl;
        }
        std::cout << "get string done: " << ser.getReadSize() << " bytes" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "\n*** " << e.what() << " ***" << std::endl;
    }
    return 0;
}