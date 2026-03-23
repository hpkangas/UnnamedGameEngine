#include "gamedata.hpp"

using json = nlohmann::json;
json GameData::init(){
    std::ifstream f("example.json");
    json data = json::parse(f);
    json asd = data.at("player");
    return asd;
}