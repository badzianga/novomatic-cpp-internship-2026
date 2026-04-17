#include <fstream>
#include "../include/FileParser.hpp"

std::vector<LogEntry> FileParser::parse(const std::string& filePath) {
    std::vector<LogEntry> entries;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            entries.push_back(parseLine(line));
        }
    }
    file.close();

    return entries;
}

LogEntry FileParser::parseLine(const std::string& line) {
    LogEntry entry;
    size_t idx = 0;

    entry.timestamp = parseTimestamp(extractField(line, idx));
    entry.level     = extractField(line, idx);
    entry.source    = extractField(line, idx);
    entry.message   = line.substr(idx + 1);

    return entry;
}

std::string FileParser::extractField(const std::string& line, size_t& startIdx) {
    const size_t start = line.find('[', startIdx);
    const size_t end = line.find(']', start);
    if (start == std::string::npos || end == std::string::npos) {
        throw std::runtime_error("Failed to extract field from line: " + line);
    }
    const std::string field = line.substr(start + 1, end - start - 1);
    startIdx = end + 1;
    return field;
}
