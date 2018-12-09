//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
#pragma once
#include "serializer.hpp"

#include <cstdint>
#include <vector>
#include <string>
#include <array>
#include <nlohmann/json.hpp>
#include <memory>
#include <sol/sol.hpp>
namespace Sample {

//
class Test {
public:
  enum class BeerType : uint8_t {
    Ales,
    Larger,
    Pilsner,
    Lambic,
    IPA,
  };

  // child class

//
struct Note {
  // members
  int page;
  int line;
  // constructor
  Note() {
    setPage(0);
    setLine(0);
  }
  //
  bool operator == (const Note& other) const {
    if (page != other.page) return false;
    if (line != other.line) return false;
    return true;
  }
  bool operator != (const Note& other) const {
    return !(*this == other);
  }
  //
  void copyFrom(const Note& other) {
    page = other.page;
    line = other.line;
  }
  Note& operator=(const Note& other) {
    copyFrom(other);
    return *this;
  }
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
  // constructor
  Child() {
    setName("father");
    setAge(50);
    setStep(20);
  }
  //
  bool operator == (const Child& other) const {
    if (bit_field.age != other.bit_field.age) return false;
    if (bit_field.step != other.bit_field.step) return false;
    if (name != other.name) return false;
    return true;
  }
  bool operator != (const Child& other) const {
    return !(*this == other);
  }
  //
  void copyFrom(const Child& other) {
    bit_field = other.bit_field;
    name = other.name;
  }
  Child& operator=(const Child& other) {
    copyFrom(other);
    return *this;
  }
  // interface
  //
  unsigned getAge() const { return bit_field.age * 1 + 18; }
  void setAge(unsigned n) { bit_field.age = (n - 18) / 1; }
  //
  unsigned getStep() const { return bit_field.step * 5 + 0; }
  void setStep(unsigned n) { bit_field.step = (n - 0) / 5; }
  //
  const std::string& getName() const { return name; }
  void setName(std::string n) { name = n; }
};

//
struct Entry {
  // members
  std::string name;
  std::string country;
  uint16_t point;
  uint8_t wins;
  //
  bool operator == (const Entry& other) const {
    if (name != other.name) return false;
    if (country != other.country) return false;
    if (point != other.point) return false;
    if (wins != other.wins) return false;
    return true;
  }
  bool operator != (const Entry& other) const {
    return !(*this == other);
  }
  //
  void copyFrom(const Entry& other) {
    name = other.name;
    country = other.country;
    point = other.point;
    wins = other.wins;
  }
  Entry& operator=(const Entry& other) {
    copyFrom(other);
    return *this;
  }
  // interface
  //
  const std::string& getName() const { return name; }
  void setName(std::string n) { name = n; }
  //
  const std::string& getCountry() const { return country; }
  void setCountry(std::string n) { country = n; }
  //
  const uint16_t getPoint() const { return point; }
  void setPoint(uint16_t n) { point = n; }
  //
  const uint8_t getWins() const { return wins; }
  void setWins(uint8_t n) { wins = n; }
};

protected:
  struct BitField {
    unsigned index : 5;
    unsigned beer_type : 5;
    signed   generation : 3;
    unsigned enabled : 1;
  };
  BitField bit_field;
  // members
  int count;
  uint32_t max_speed;
  std::vector<uint8_t> ranking;
  std::vector<float> line;
  std::array<int32_t, 8> line2;
  std::array<Note, 4> note;
  Child child;
  std::vector<Entry> entry_list;
public:
  // constructor
  Test() {
    ranking.resize(32);
    line.resize(8);
    setIndex(10);
    setBeerType(BeerType::IPA);
    setGeneration(0);
    setEnabled(false);
    setCount(0);
    setMaxSpeed(320);
  }
  //
  bool operator == (const Test& other) const {
    if (bit_field.index != other.bit_field.index) return false;
    if (bit_field.beer_type != other.bit_field.beer_type) return false;
    if (bit_field.generation != other.bit_field.generation) return false;
    if (bit_field.enabled != other.bit_field.enabled) return false;
    if (count != other.count) return false;
    if (max_speed != other.max_speed) return false;
    if (ranking.size() != other.ranking.size()) return false;
    for (size_t i = 0; i < ranking.size(); i++)
    {
      if (ranking[i] != other.ranking[i]) return false;
    }
    if (line.size() != other.line.size()) return false;
    for (size_t i = 0; i < line.size(); i++)
    {
      if (line[i] != other.line[i]) return false;
    }
    for (size_t i = 0; i < line2.size(); i++)
    {
      if (line2[i] != other.line2[i]) return false;
    }
    for (size_t i = 0; i < note.size(); i++)
    {
      if (note[i] != other.note[i]) return false;
    }
    if (child != other.child) return false;
    if (entry_list.size() != other.entry_list.size()) return false;
    for (size_t i = 0; i < entry_list.size(); i++)
    {
      if (entry_list[i] != other.entry_list[i]) return false;
    }
    return true;
  }
  bool operator != (const Test& other) const {
    return !(*this == other);
  }
  //
  void copyFrom(const Test& other) {
    bit_field = other.bit_field;
    count = other.count;
    max_speed = other.max_speed;
    ranking = other.ranking;
    line = other.line;
    line2 = other.line2;
    note = other.note;
    child = other.child;
    entry_list = other.entry_list;
  }
  Test& operator=(const Test& other) {
    copyFrom(other);
    return *this;
  }
  //
  void serialize(Serializer& ser);
  void deserialize(Serializer& ser);
  size_t getSerializeSize() const;
  //
  void serializeJSON(nlohmann::json& json);
  void deserializeJSON(nlohmann::json& json);
  static const char* getStringBeerType(BeerType n);
  //
  static void setLUA(sol::state& lua);
  // interface
  //
  unsigned getIndex() const { return bit_field.index * 1 + 0; }
  void setIndex(unsigned n) { bit_field.index = (n - 0) / 1; }
  //
  BeerType getBeerType() const { return static_cast<BeerType>(bit_field.beer_type); }
  void setBeerType(BeerType n) { bit_field.beer_type = static_cast<unsigned>(n); }
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
  const uint32_t getMaxSpeed() const { return max_speed; }
  void setMaxSpeed(uint32_t n) { max_speed = n; }
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
  const int32_t getLine2(int idx) const { return line2[idx]; }
  void setLine2(int idx, int32_t n) { line2[idx] = n; }
  size_t getLine2Size() const { return line2.size(); }
  //
  const Note& getNote(int idx) const { return note[idx]; }
  void setNote(int idx, Note& n) { note[idx] = n; }
  size_t getNoteSize() const { return note.size(); }
  //
  const Child& getChild() const { return child; }
  void setChild(Child& n) { child = n; }
  //
  const Entry& getEntryList(int idx) const { return entry_list[idx]; }
  void setEntryList(int idx, Entry& n) { entry_list[idx] = n; }
  size_t getEntryListSize() const { return entry_list.size(); }
  void appendEntryList(Entry& n) { entry_list.emplace_back(n); }
  void resizeEntryList(size_t sz) { entry_list.resize(sz); }
};
} // namespace Sample
