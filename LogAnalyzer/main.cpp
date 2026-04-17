#include <iostream>
#include <vector>
#include "FileParser.hpp"
#include "LogEntry.hpp"
#include "QueryParser.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file>\n";
        return 0;
    }

    std::vector<LogEntry> entries;

    try {
        entries = FileParser::parse(argv[1]);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    QueryParser queryParser;
    for (;;) {
        try {
            std::cout << "> ";
            std::string line;
            std::getline(std::cin, line);

            if (line.empty()) return 0;

            const auto query = queryParser.parse(line);
            for (const auto& entry : entries) {
                if (query->evaluate(entry)) {
                    std::cout << entry << '\n';
                }
            }
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
