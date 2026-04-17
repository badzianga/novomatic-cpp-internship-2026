#ifndef LOGANALYZER_TOKEN_HPP
#define LOGANALYZER_TOKEN_HPP
#include <string>

struct Token {
    enum class Type {
        END,
        TIMESTAMP, LOG_LEVEL, SOURCE, MESSAGE, STRING,
        EQUALS, NOT_EQUALS, FROM, TO,
        AND, OR,
        LEFT_PAREN, RIGHT_PAREN
    };

    Type type = Type::END;
    std::string_view value;
};

#endif //LOGANALYZER_TOKEN_HPP
