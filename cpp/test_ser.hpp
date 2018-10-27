//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
#pragma once

#include "test.hpp"
#include "serializer.hpp"

namespace Sample {
// total data size
size_t getTestPackSize(const Test& s);
// pack interface
void packTest(Serializer& ser, Test& s);
// unpack interface
void unpackTest(Serializer& ser, Test& s);
} // namespace Sample
