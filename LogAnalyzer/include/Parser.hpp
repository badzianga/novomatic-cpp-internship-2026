#ifndef LOGANALYZER_PARSER_HPP
#define LOGANALYZER_PARSER_HPP
#include <chrono>

class Parser {
public:
    static std::chrono::system_clock::time_point parseTimestamp(const std::string& ts);
};

#endif //LOGANALYZER_PARSER_HPP
