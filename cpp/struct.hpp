//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
#pragma once
#include "test_def.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace Sample {

//
class Test {
public:
  // child class

  //
  struct Note {
    // members
    int page;
    int line;
    // interface
    //
    const int getPage() const { return page; }
    void setPage(int n) { page = n; }
    //
    const int getLine() const { return line; }
    void setLine(int n) { line = n; }
  };

  //
  struct Child {
    struct BitField {
      unsigned age : 6;
      unsigned step : 4;
    };
    BitField bit_field;
    // members
    std::string name;
    // interface
    //
    unsigned getAge() const { return bit_field.age * 1 + 18; }
    void setAge(unsigned n) { bit_field.age = (n - 18) / 1; }
    //
    unsigned getStep() const { return bit_field.step * 5 + 0; }
    void setStep(unsigned n) { bit_field.step = (n - 0) / 5; }
    //
    const std::string getName() const { return name; }
    void setName(std::string n) { name = n; }
  };

  //
  struct Entry {
    // members
    std::string name;
    std::string country;
    // interface
    //
    const std::string getName() const { return name; }
    void setName(std::string n) { name = n; }
    //
    const std::string getCountry() const { return country; }
    void setCountry(std::string n) { country = n; }
  };

protected:
  struct BitField {
    unsigned index : 5;
    unsigned beer_type : 7;
    signed generation : 3;
    unsigned enabled : 1;
  };
  BitField bit_field;
  // members
  int count;
  std::vector<uint8_t> ranking;
  std::vector<float> line;
  std::array<Note, 4> note;
  Child child;
  std::vector<Entry> entry_list;

public:
  // constructor
  Test() {
    ranking.resize(32);
    line.resize(8);
  }

  // interface
  //
  unsigned getIndex() const { return bit_field.index * 1 + 0; }
  void setIndex(unsigned n) { bit_field.index = (n - 0) / 1; }
  //
  BeerType getBeerType() const {
    return static_cast<BeerType>(bit_field.beer_type);
  }
  void setBeerType(BeerType n) {
    bit_field.beer_type = static_cast<unsigned>(n);
  }
  //
  signed getGeneration() const { return bit_field.generation * 1 + 0; }
  void setGeneration(signed n) { bit_field.generation = (n - 0) / 1; }
  //
  bool getEnabled() const { return bit_field.enabled; }
  void setEnabled(bool f) { bit_field.enabled = f; }
  //
  const int getCount() const { return count; }
  void setCount(int n) { count = n; }
  //
  const uint8_t getRanking(int idx) const { return ranking[idx]; }
  void setRanking(int idx, uint8_t n) { ranking[idx] = n; }
  size_t getRankingSize() const { return ranking.size(); }
  void appendRanking(uint8_t n) { ranking.emplace_back(n); }
  void resizeRanking(size_t sz) { ranking.resize(sz); }
  //
  const float getLine(int idx) const { return line[idx]; }
  void setLine(int idx, float n) { line[idx] = n; }
  size_t getLineSize() const { return line.size(); }
  void appendLine(float n) { line.emplace_back(n); }
  void resizeLine(size_t sz) { line.resize(sz); }
  //
  const Note &getNote(int idx) const { return note[idx]; }
  void setNote(int idx, Note &n) { note[idx] = n; }
  size_t getNoteSize() const { return note.size(); }
  //
  const Child &getChild() const { return child; }
  void setChild(Child &n) { child = n; }
  //
  const Entry &getEntryList(int idx) const { return entry_list[idx]; }
  void setEntryList(int idx, Entry &n) { entry_list[idx] = n; }
  size_t getEntryListSize() const { return entry_list.size(); }
  void appendEntryList(Entry &n) { entry_list.emplace_back(n); }
  void resizeEntryList(size_t sz) { entry_list.resize(sz); }
};
} // namespace Sample
