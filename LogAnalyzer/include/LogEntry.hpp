#ifndef LOGANALYZER_LOGENTRY_HPP
#define LOGANALYZER_LOGENTRY_HPP
#include <chrono>
#include <string>

struct LogEntry {
    std::chrono::system_clock::time_point timestamp;
    std::string level;
    std::string source;
    std::string message;

    friend std::ostream& operator<<(std::ostream& os, const LogEntry& entry);
};

#endif //LOGANALYZER_LOGENTRY_HPP
