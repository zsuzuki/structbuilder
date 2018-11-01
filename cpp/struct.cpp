//
// struct input/output test
//

#include "struct.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

using BeerType = Sample::Test::BeerType;

void checkBeer(const Sample::Test &t) {
  switch (t.getBeerType()) {
  case BeerType::Ales:
    std::cout << "Ales" << std::endl;
    break;
  case BeerType::Lambic:
    std::cout << "Lambic" << std::endl;
    break;
  default:
    std::cout << "Other" << std::endl;
    break;
  }
}

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

  checkBeer(test);

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
