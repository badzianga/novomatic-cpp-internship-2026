#ifndef BLUEPRINTAUTOLAYOUT_JSONREADER_HPP
#define BLUEPRINTAUTOLAYOUT_JSONREADER_HPP
#include <nlohmann/json_fwd.hpp>

class JsonReader {
public:
    static nlohmann::json readFromFile(std::string filePath);
};

#endif //BLUEPRINTAUTOLAYOUT_JSONREADER_HPP
