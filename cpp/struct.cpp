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
  lua["gTest"] = &test;
  lua.script_file("lua/struct.lua");

  Sample::Test t2;
  {
    json j;
    test.serializeJSON(j);
    std::ofstream ofile{"save.json"};
    ofile << j;
    t2.deserializeJSON(j);
  }
  std::cout << "Name: \"" << t2.getChild().getName()
            << "\", age=" << t2.getChild().getAge()
            << ", step=" << t2.getChild().getStep() << std::endl;

  return 0;
}
