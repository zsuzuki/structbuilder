//
// this file is auto generated
// by structbuilder<https://github.com/zsuzuki/structbuilder>
//


#include "struct.hpp"


#include <map>

using json = nlohmann::json;

namespace Sample {

namespace {
//
const char* enum_BeerType_list[] = {
    "Ales", "Larger", "Pilsner", "Lambic", "IPA",
};
const std::map<std::string, Test::BeerType> enum_BeerType_map = {
    { "Ales", Test::BeerType::Ales },
    { "Larger", Test::BeerType::Larger },
    { "Pilsner", Test::BeerType::Pilsner },
    { "Lambic", Test::BeerType::Lambic },
    { "IPA", Test::BeerType::IPA },
};
} // namespace

//
void Test::serializeJSON(json& j) {
    json jsonObject;

    jsonObject["index"] = (unsigned)bit_field.index;
    jsonObject["beer_type"] = enum_BeerType_list[(int)bit_field.beer_type];
    jsonObject["generation"] = (signed)bit_field.generation;
    jsonObject["enabled"] = (unsigned)bit_field.enabled;
    jsonObject["count"] = count;
    jsonObject["max_speed"] = max_speed;
    for (auto& tRanking : ranking) {
        jsonObject["ranking"].push_back(tRanking);
    }
    for (auto& tLine : line) {
        jsonObject["line"].push_back(tLine);
    }
    for (auto& tNote : note) {
        json     jNote;
        jNote["page"] = tNote.page;
        jNote["line"] = tNote.line;
        jsonObject["note"].push_back(jNote);
    }
    jsonObject["child"]["age"] = (unsigned)child.bit_field.age;
    jsonObject["child"]["step"] = (unsigned)child.bit_field.step;
    jsonObject["child"]["name"] = child.name;
    for (auto& tEntryList : entry_list) {
        json     jEntryList;
        jEntryList["name"] = tEntryList.name;
        jEntryList["country"] = tEntryList.country;
        jEntryList["point"] = tEntryList.point;
        jEntryList["wins"] = tEntryList.wins;
        jsonObject["entry_list"].push_back(jEntryList);
    }
    //
    j["Test"] = jsonObject;
}
//
void Test::deserializeJSON(json& j) {
    json jsonReader = j["Test"];

    if (!jsonReader["index"].is_null())
        bit_field.index = jsonReader["index"];
    if (!jsonReader["beer_type"].is_null())
    bit_field.beer_type = static_cast<unsigned>
        (enum_BeerType_map.at(jsonReader["beer_type"].get<std::string>()));
    if (!jsonReader["generation"].is_null())
        bit_field.generation = jsonReader["generation"];
    if (!jsonReader["enabled"].is_null())
        bit_field.enabled = jsonReader["enabled"];
    if (!jsonReader["count"].is_null()) {
    count = jsonReader["count"];
    }
    if (!jsonReader["max_speed"].is_null()) {
    max_speed = jsonReader["max_speed"];
    }
    if (!jsonReader["ranking"].is_null()) {
    json jRanking = jsonReader["ranking"];
    ranking.reserve(jRanking.size());
    ranking.resize(0);
    for (auto& jRankingIt : jRanking) {
        ranking.push_back(jRankingIt);
    }
    }
    if (!jsonReader["line"].is_null()) {
    json jLine = jsonReader["line"];
    line.reserve(jLine.size());
    line.resize(0);
    for (auto& jLineIt : jLine) {
        line.push_back(jLineIt);
    }
    }
    if (!jsonReader["note"].is_null()) {
    json jNote = jsonReader["note"];
    int jNoteIndex = 0;
    for (auto& jNoteIt : jNote) {
      if (jNoteIndex < 4) {
        auto& tObj = note[jNoteIndex];
    if (!jNoteIt["page"].is_null()) {
    tObj.page = jNoteIt["page"];
    }
    if (!jNoteIt["line"].is_null()) {
    tObj.line = jNoteIt["line"];
    }
        jNoteIndex++;}
    }
    }
    if (!jsonReader["child"].is_null()) {
    if (!jsonReader["child"]["age"].is_null())
        child.bit_field.age = jsonReader["child"]["age"];
    if (!jsonReader["child"]["step"].is_null())
        child.bit_field.step = jsonReader["child"]["step"];
    if (!jsonReader["child"]["name"].is_null()) {
    child.name = jsonReader["child"]["name"];
    }
    }
    if (!jsonReader["entry_list"].is_null()) {
    json jEntryList = jsonReader["entry_list"];
    entry_list.reserve(jEntryList.size());
    entry_list.resize(0);
    for (auto& jEntryListIt : jEntryList) {Entry tObj{};
    if (!jEntryListIt["name"].is_null()) {
    tObj.name = jEntryListIt["name"];
    }
    if (!jEntryListIt["country"].is_null()) {
    tObj.country = jEntryListIt["country"];
    }
    if (!jEntryListIt["point"].is_null()) {
    tObj.point = jEntryListIt["point"];
    }
    if (!jEntryListIt["wins"].is_null()) {
    tObj.wins = jEntryListIt["wins"];
    }
        entry_list.emplace_back(tObj);
    }
    }
}

} // namespace Sample
