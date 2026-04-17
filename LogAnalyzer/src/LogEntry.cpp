#include "LogEntry.hpp"
#include <ostream>

std::ostream& operator<<(std::ostream& os, const LogEntry& entry) {
    os << '[' << std::format("{:%FT%T}", std::chrono::floor<std::chrono::seconds>(entry.timestamp))
       << "] [" << entry.level
       << "] [" << entry.source
       << "]: " << entry.message;
    return os;
}
