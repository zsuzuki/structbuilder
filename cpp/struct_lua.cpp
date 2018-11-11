//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//
#include <sol/sol.hpp>

#include "struct.hpp"


namespace Sample {
void Test::setLUA(sol::state& lua)
{
  lua.new_usertype<Note>(
    "TestNote",
    "page", &Note::page,
    "line", &Note::line,
    "copyFrom", &Note::copyFrom);
  lua.new_usertype<Child>(
    "TestChild",
    "age", sol::property(&Child::getAge, &Child::setAge),
    "step", sol::property(&Child::getStep, &Child::setStep),
    "name", &Child::name,
    "copyFrom", &Child::copyFrom);
  lua.new_usertype<Entry>(
    "TestEntry",
    "name", &Entry::name,
    "country", &Entry::country,
    "point", &Entry::point,
    "wins", &Entry::wins,
    "copyFrom", &Entry::copyFrom);
  lua.new_usertype<Test>(
    "Test",
    "index", sol::property(&Test::getIndex, &Test::setIndex),
    "beer_type", sol::property(&Test::getBeerType, &Test::setBeerType),
    "generation", sol::property(&Test::getGeneration, &Test::setGeneration),
    "enabled", sol::property(&Test::getEnabled, &Test::setEnabled),
    "count", &Test::count,
    "max_speed", &Test::max_speed,
    "ranking", &Test::ranking,
    "line", &Test::line,
    "note", &Test::note,
    "child", &Test::child,
    "entry_list", &Test::entry_list,
    "copyFrom", &Test::copyFrom);
  sol::table t_BeerType = lua.create_table_with();
  t_BeerType["Ales"] = (int)BeerType::Ales;
  t_BeerType["Larger"] = (int)BeerType::Larger;
  t_BeerType["Pilsner"] = (int)BeerType::Pilsner;
  t_BeerType["Lambic"] = (int)BeerType::Lambic;
  t_BeerType["IPA"] = (int)BeerType::IPA;
  lua["Test"]["BeerType"] = t_BeerType;
}

} // namespace Sample
