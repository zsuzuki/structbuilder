//
//
//

#include "struct.hpp"
#include <iostream>

int main(int argc, char **argv) {
  Sample::Test test1, test2;

  auto ch = test1.getChild();
  ch.setAge(80);
  ch.setStep(42);
  test1.setChild(ch);

  auto e = Sample::Test::Entry{};
  e.setName("OK");
  test2.appendEntryList(e);

  std::cout << ch.getAge() << "/" << ch.getStep() << ", " << e.getName()
            << std::endl;

  return 0;
}
