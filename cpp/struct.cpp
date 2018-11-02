//
// struct input/output test
//

#include "struct.hpp"
#include <fstream>
#include <iostream>
#include <sol/sol.hpp>

using json = nlohmann::json;

int main(int argc, char **argv) {
  Sample::Test test;
  {
    std::ifstream ifile("save.json");
    json ij;
    ifile >> ij;
    test.deserializeJSON(ij);
  }
  auto cnt = test.getCount() + 1;
  test.setCount(cnt);
  std::cout << "Execute count: " << cnt << std::endl;

  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine,
                     sol::lib::string, sol::lib::math, sol::lib::table,
                     sol::lib::debug, sol::lib::bit32);
  test.setLUA(lua);
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++)
    args.emplace_back(argv[i]);
  lua["args"] = args;
  lua["gTest"] = &test;
  lua.script_file("lua/struct.lua");

  {
    json j;
    test.serializeJSON(j);
    std::ofstream ofile{"save.json"};
    ofile << j;
  }

  return 0;
}
