#include <stdexcept>
#include "Lexer.hpp"

Lexer::Lexer(const std::string& input) : cur(input.begin()), end(input.end()) {}

Token Lexer::nextToken() {
    skipWhitespace();

    if (cur == end) {
        return { Token::Type::END, "" };
    }

    const char c = *cur;

    switch (c) {
        case '"': {
            const auto start = ++cur;  // consume "
            while (cur != end && *cur != '"') {
                ++cur;
            }
            if (cur == end) {
                throw std::runtime_error("Unterminated string");
            }
            const std::string_view value(start, cur);
            ++cur;  // consume "
            return { Token::Type::STRING, value };
        }
        case '(': {
            const auto start = cur++;
            const std::string_view value(start, cur);
            return { Token::Type::LEFT_PAREN, value };
        }
        case ')': {
            const auto start = cur++;
            const std::string_view value(start, cur);
            return { Token::Type::RIGHT_PAREN, value };
        }
        case '=': {
            const auto start = cur++;
            const std::string_view value(start, cur);
            return { Token::Type::EQUALS, value };
        }
        case '!': {
            const auto start = cur++;
            if (*cur != '=') {
                throw std::runtime_error("Missing '=' after '!'");
            }
            ++cur;  // consume =
            const std::string_view value(start, cur);
            return { Token::Type::NOT_EQUALS, value };
        }
        default: break;
    }

    if (isalpha(c) || c == '_') {
        const auto start = cur++;

        while (cur != end && (isalpha(*cur) || *cur == '_')) {
            ++cur;
        }

        const std::string_view word(start, cur);

        if (equalsIgnoreCase(word, "OD"))   return { Token::Type::FROM, word };
        if (equalsIgnoreCase(word, "DO"))   return { Token::Type::TO, word };
        if (equalsIgnoreCase(word, "LUB"))  return { Token::Type::OR, word };
        if (equalsIgnoreCase(word, "ORAZ")) return { Token::Type::AND, word };
        if (equalsIgnoreCase(word, "TIMESTAMP")) return { Token::Type::TIMESTAMP, word };
        if (equalsIgnoreCase(word, "LOG_LEVEL")) return { Token::Type::LOG_LEVEL, word };
        if (equalsIgnoreCase(word, "SOURCE")) return { Token::Type::SOURCE, word };
        if (equalsIgnoreCase(word, "MESSAGE")) return { Token::Type::MESSAGE, word };

        throw std::runtime_error(std::string("Unknown keyword: ") + std::string(word));
    }

    throw std::runtime_error(std::string("Unknown character: ") + *cur);
}

void Lexer::skipWhitespace() {
    while (cur != end && std::isspace(*cur)) {
        ++cur;
    }
}

bool Lexer::equalsIgnoreCase(std::string_view word, std::string_view expected) {
    if (word.length() != expected.length()) return false;

    for (size_t i = 0; i < word.length(); ++i) {
        if (std::toupper(static_cast<unsigned char>(word[i])) != std::toupper(static_cast<unsigned char>(expected[i]))) {
            return false;
        }
    }
    return true;
}
