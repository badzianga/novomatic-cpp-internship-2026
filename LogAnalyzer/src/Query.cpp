#include "Query.hpp"

LogicalConnectorQuery::LogicalConnectorQuery(std::unique_ptr<Query> left, std::unique_ptr<Query> right)
    : left(std::move(left)), right(std::move(right)) {}

EqualityQuery::EqualityQuery(Token::Type field, std::string_view expected)
    : field(field), expected{expected} {}

bool AndQuery::evaluate(const LogEntry& entry) const {
    return left->evaluate(entry) && right->evaluate(entry);
}

bool OrQuery::evaluate(const LogEntry& entry) const {
    return left->evaluate(entry) || right->evaluate(entry);
}

bool EqualsQuery::evaluate(const LogEntry& entry) const {
    switch (field) {
        case Token::Type::SOURCE:    return entry.source == expected;
        case Token::Type::LOG_LEVEL: return entry.level == expected;
        case Token::Type::MESSAGE:   return entry.message.find(expected) != std::string::npos;
        default: return false;
    }
}

bool NotEqualsQuery::evaluate(const LogEntry& entry) const {
    switch (field) {
        case Token::Type::SOURCE:    return entry.source != expected;
        case Token::Type::LOG_LEVEL: return entry.level != expected;
        case Token::Type::MESSAGE:   return entry.message.find(expected) == std::string::npos;
        default: return false;
    }
}

TimeRangeQuery::TimeRangeQuery(std::chrono::system_clock::time_point fromTp, std::chrono::system_clock::time_point toTp)
    : from(fromTp), to(toTp) {}

bool TimeRangeQuery::evaluate(const LogEntry& entry) const {
    return entry.timestamp >= from && entry.timestamp <= to;
}
