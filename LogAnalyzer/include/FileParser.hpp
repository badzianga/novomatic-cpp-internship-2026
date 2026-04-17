#ifndef LOGANALYZER_FILEPARSER_HPP
#define LOGANALYZER_FILEPARSER_HPP
#include <vector>
#include "LogEntry.hpp"
#include "Parser.hpp"

class FileParser : public Parser {
public:
    static std::vector<LogEntry> parse(const std::string& filePath);
private:
    static LogEntry parseLine(const std::string& line);
    static std::string extractField(const std::string& line, size_t& idx);
};

#endif //LOGANALYZER_FILEPARSER_HPP
