//
//
//

#include "struct.hpp"
#include <iostream>

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

  test1.setBeerType(BeerType::Pilsner);
  test2.setBeerType(BeerType::Lambic);
  auto ch = test1.getChild();
  ch.setAge(80);
  ch.setStep(42);
  test1.setChild(ch);

  auto e = Sample::Test::Entry{};
  e.setName("OK");
  test2.appendEntryList(e);

  std::cout << ch.getAge() << "/" << ch.getStep() << ", " << e.getName()
            << std::endl;

  checkBeer(test1);
  checkBeer(test2);
  return 0;
}
