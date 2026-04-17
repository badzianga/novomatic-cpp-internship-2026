#ifndef LOGANALYZER_QUERY_HPP
#define LOGANALYZER_QUERY_HPP
#include <chrono>
#include <memory>
#include "LogEntry.hpp"
#include "Token.hpp"

struct Query {
    virtual ~Query() = default;
    [[nodiscard]] virtual bool evaluate(const LogEntry& entry) const = 0;
};

struct LogicalConnectorQuery : Query {
    LogicalConnectorQuery(std::unique_ptr<Query> left, std::unique_ptr<Query> right);

    std::unique_ptr<Query> left;
    std::unique_ptr<Query> right;
};

struct EqualityQuery : Query {
    EqualityQuery(Token::Type field, std::string_view expected);

    Token::Type field;
    std::string_view expected;
};

struct AndQuery : LogicalConnectorQuery {
    using LogicalConnectorQuery::LogicalConnectorQuery;
    [[nodiscard]] bool evaluate(const LogEntry& entry) const override;
};

struct OrQuery : LogicalConnectorQuery {
    using LogicalConnectorQuery::LogicalConnectorQuery;
    [[nodiscard]] bool evaluate(const LogEntry& entry) const override;
};

struct EqualsQuery : EqualityQuery {
    using EqualityQuery::EqualityQuery;
    [[nodiscard]] bool evaluate(const LogEntry& entry) const override;
};

struct NotEqualsQuery : EqualityQuery {
    using EqualityQuery::EqualityQuery;
    [[nodiscard]] bool evaluate(const LogEntry& entry) const override;
};

struct TimeRangeQuery : Query {
    TimeRangeQuery(std::chrono::system_clock::time_point fromTp, std::chrono::system_clock::time_point toTp);
    [[nodiscard]] bool evaluate(const LogEntry& entry) const override;

    std::chrono::system_clock::time_point from;
    std::chrono::system_clock::time_point to;
};

#endif //LOGANALYZER_QUERY_HPP
