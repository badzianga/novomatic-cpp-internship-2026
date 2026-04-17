#include "Parser.hpp"

std::chrono::system_clock::time_point Parser::parseTimestamp(const std::string& ts) {
    if (ts.empty()) {
        return std::chrono::system_clock::now();
    }

    std::tm tm{};

    std::sscanf(ts.c_str(), "%d-%d-%dT%d:%d:%d",
        &tm.tm_year,
        &tm.tm_mon,
        &tm.tm_mday,
        &tm.tm_hour,
        &tm.tm_min,
        &tm.tm_sec);

    tm.tm_year -= 1900;
    tm.tm_mon  -= 1;

    const std::time_t tt = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(tt);
}
