#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "JsonReader.hpp"

nlohmann::json JsonReader::readFromFile(std::string filePath) {
    std::ifstream file(filePath);
    if (!file) {
        throw std::runtime_error("File not found");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string src = buffer.str();
    file.close();

    return nlohmann::json::parse(src);
}
