//
// struct input/output test
//

#include "struct.hpp"
#include "serializer.hpp"
#include <fstream>
#include <iostream>
#include <sol/sol.hpp>

using json = nlohmann::json;

int main(int argc, char **argv) {
  Sample::Test test;
  bool load_json = false;
  {
    std::ifstream ifile("save.json");
    if (ifile.good()) {
      json ij;
      ifile >> ij;
      test.deserializeJSON(ij);
      load_json = true;
    }
  }
  test.setCount(test.getCount() + 1);

  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine,
                     sol::lib::string, sol::lib::math, sol::lib::table,
                     sol::lib::debug, sol::lib::bit32);
  test.setLUA(lua);
  std::string lua_file = "lua/struct.lua";
  std::vector<std::string> args;
  bool skip = false;
  for (int i = 1; i < argc; i++) {
    std::string a{argv[i]};
    if (a == "-lua" && i + 1 < argc) {
      lua_file = argv[i + 1];
      skip = true;
    } else {
      if (!skip)
        args.emplace_back(argv[i]);
      skip = false;
    }
  }
  lua["ExecuteFilename"] = lua_file;
  lua["InitializedTest"] = load_json;
  lua["args"] = args;
  lua["gTest"] = &test;
  try {
    lua.script_file(lua_file);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  {
    Serializer ser;
    uint8_t buffer[1024];
    ser.initialize(buffer, sizeof(buffer));
    test.serialize(ser);
    std::cout << "Binary Size: " << test.getSerializeSize() << "/"
              << ser.getWriteSize() << std::endl;
    Sample::Test test2;
    test2.deserialize(ser);
    json j;
    test2.serializeJSON(j);
    std::ofstream ofile{"save2.json"};
    ofile << j;
  }

  {
    json j;
    test.serializeJSON(j);
    std::ofstream ofile{"save.json"};
    ofile << j;
  }

  return 0;
}
