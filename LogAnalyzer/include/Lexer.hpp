#ifndef LOGANALYZER_LEXER_HPP
#define LOGANALYZER_LEXER_HPP
#include <string>
#include "Token.hpp"

class Lexer {
public:
    Lexer() = default;
    explicit Lexer(const std::string& input);

    Token nextToken();

private:
    void skipWhitespace();
    static bool equalsIgnoreCase(std::string_view word, std::string_view expected);

    std::string::const_iterator cur;
    std::string::const_iterator end;
};

#endif //LOGANALYZER_LEXER_HPP
