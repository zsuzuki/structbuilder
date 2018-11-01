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
    std::cout << "I love" << std::endl;
    break;
  case BeerType::Lambic:
    std::cout << "normal" << std::endl;
    break;
  default:
    std::cout << "I like" << std::endl;
    break;
  }
}

int main(int argc, char **argv) {
  Sample::Test test1, test2;

  {
    std::ifstream ifile("save.json");
    json ij;
    ifile >> ij;
    test1.deserializeJSON(ij);
  }

  checkBeer(test1);
  checkBeer(test2);

  json j;
  test1.serializeJSON(j);
  std::cout << j << std::endl;

  test2.deserializeJSON(j);
  std::cout << test2.getChild().getAge() << std::endl;
  std::cout << test2.getChild().getStep() << std::endl;

  std::ofstream ofile{"save.json"};
  ofile << j;

  return 0;
}
