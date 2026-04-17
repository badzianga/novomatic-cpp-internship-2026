#include "QueryParser.hpp"

std::unique_ptr<Query> QueryParser::parse(const std::string& query) {
    lexer = Lexer(query);
    currentToken = lexer.nextToken();

    return parseOr();
}

std::unique_ptr<Query> QueryParser::parseOr() {
    auto left = parseAnd();

    while (match(Token::Type::OR)) {
        auto right = parseAnd();
        left = std::make_unique<OrQuery>(std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Query> QueryParser::parseAnd() {
    auto left = parseParentheses();

    while (match(Token::Type::AND)) {
        auto right = parseParentheses();
        left = std::make_unique<AndQuery>(std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Query> QueryParser::parseParentheses() {
    if (match(Token::Type::LEFT_PAREN)) {
        auto query = parseOr();
        consume(Token::Type::RIGHT_PAREN);
        return query;
    }
    return parseCondition();
}

std::unique_ptr<Query> QueryParser::parseCondition() {
    const auto [fieldType, fieldValue] = currentToken;
    if (!(match(Token::Type::TIMESTAMP) ||
          match(Token::Type::LOG_LEVEL) ||
          match(Token::Type::SOURCE) ||
          match(Token::Type::MESSAGE))) {
        throw std::runtime_error(std::string("Unexpected token: \"") + std::string(fieldValue) + '\"');
    }

    if (match(Token::Type::EQUALS)) {
        const auto value = currentToken.value;
        consume(Token::Type::STRING);

        return std::make_unique<EqualsQuery>(fieldType, value);
    }
    if (match(Token::Type::NOT_EQUALS)) {
        const auto value = currentToken.value;
        consume(Token::Type::STRING);

        return std::make_unique<NotEqualsQuery>(fieldType, value);
    }
    if (match(Token::Type::FROM)) {
        const auto fromDate = std::string(currentToken.value);
        consume(Token::Type::STRING);
        std::string toDate;
        if (match(Token::Type::TO)) {
            toDate = std::string(currentToken.value);
            consume(Token::Type::STRING);
        }

        return std::make_unique<TimeRangeQuery>(parseTimestamp(fromDate), parseTimestamp(toDate));
    }
    throw std::runtime_error("Invalid condition");
}

void QueryParser::consume(Token::Type type) {
    if (currentToken.type != type) {
        throw std::runtime_error("Unexpected token");
    }
    currentToken = lexer.nextToken();
}

bool QueryParser::match(Token::Type type) {
    if (currentToken.type == type) {
        currentToken = lexer.nextToken();
        return true;
    }
    return false;
}
