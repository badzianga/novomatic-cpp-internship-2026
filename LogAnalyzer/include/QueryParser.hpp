#ifndef LOGANALYZER_QUERYPARSER_HPP
#define LOGANALYZER_QUERYPARSER_HPP
#include <memory>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Query.hpp"

class QueryParser : public Parser {
public:
    std::unique_ptr<Query> parse(const std::string& query);
private:
    std::unique_ptr<Query> parseOr();
    std::unique_ptr<Query> parseAnd();
    std::unique_ptr<Query> parseParentheses();
    std::unique_ptr<Query> parseCondition();

    void consume(Token::Type type);
    bool match(Token::Type type);

    Lexer lexer;
    Token currentToken;
};

#endif //LOGANALYZER_QUERYPARSER_HPP
