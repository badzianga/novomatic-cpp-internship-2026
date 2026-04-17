#include <gtest/gtest.h>
#include "LogEntry.hpp"
#include "Parser.hpp"
#include "QueryParser.hpp"

// These tests are ugly, but I was sleepy

static std::vector<LogEntry> prepEntries() {
    return {
        LogEntry(Parser::parseTimestamp("2023-10-25T10:26:10"), "INFO", "AuthService", "User session refreshed"),
        LogEntry(Parser::parseTimestamp("2023-10-25T10:27:05"), "WARN", "API", "High latency detected"),
        LogEntry(Parser::parseTimestamp("2023-10-25T10:28:20"), "ERROR", "Database", "Connection timeout"),
        LogEntry(Parser::parseTimestamp("2023-10-25T10:29:00"), "INFO", "Payment", "Invoice generated 12346"),
        LogEntry(Parser::parseTimestamp("2023-10-25T10:30:15"), "WARN", "AuthService", "Password attempt limit nearing"),
    };
}

static void assertEntries(const std::unique_ptr<Query>& query, std::vector<bool> conditionsMet) {
    const auto entries = prepEntries();

    for (int i = 0; i < prepEntries().size(); ++i) {
        if (conditionsMet[i]) {
            ASSERT_TRUE(query->evaluate(entries[i]));
        }
        else {
            ASSERT_FALSE(query->evaluate(entries[i]));
        }
    }
}

TEST(LogAnalyzer, shouldThrowErrorWhenStringIsUnterminated) {
    QueryParser queryParser;
    const std::string q = R"("SOURCE = "AuthService)";
    ASSERT_THROW(queryParser.parse(q), std::runtime_error);
}

TEST(LogAnalyzer, shouldThrowErrorWhenUnexpectedTokenIsPresent) {
    QueryParser queryParser;
    const std::string q = R"("Hello")";
    ASSERT_THROW(queryParser.parse(q), std::runtime_error);
}

TEST(LogAnalyzer, shouldThrowErrorWhenInvalidConditionIsWritten) {
    QueryParser queryParser;
    const std::string q = "MESSAGE";
    ASSERT_THROW(queryParser.parse(q), std::runtime_error);
}

TEST(LogAnalyzer, shouldReturnLogsWithSourceCondition) {
    const auto entries = prepEntries();
    QueryParser queryParser;
    const std::string q = R"(SOURCE = "AuthService")";
    const auto query = queryParser.parse(q);
    assertEntries(query, {true, false, false, false, true});
}

TEST(LogAnalyzer, shouldReturnLogsWithLogLevelCondition) {
    QueryParser queryParser;
    const std::string q = R"(LOG_LEVEL = "WARN")";
    const auto query = queryParser.parse(q);
    assertEntries(query, {false, true, false, false, true});
}

TEST(LogAnalyzer, shouldReturnLogsWithMessageCondition) {
    QueryParser queryParser;
    std::string q = R"(Message = "User")";
    const auto query = queryParser.parse(q);
    assertEntries(query, {true, false, false, false, false});
}

TEST(LogAnalyzer, shouldReturnLogsWithTimestampCondition) {
    QueryParser queryParser;
    const std::string q = R"(TIMESTAMP OD "2023-10-25T10:27:00" DO "2023-10-25T10:29:30")";
    const auto query = queryParser.parse(q);
    assertEntries(query, {false, true, true, true, false});
}

TEST(LogAnalyzer, shouldReturnLogsWithPartialTimestampCondition) {
    QueryParser queryParser;
    const std::string q = R"(TIMESTAMP OD "2023-10-25T10:28:00")";
    const auto query = queryParser.parse(q);
    assertEntries(query, {false, false, true, true, true});
}

TEST(LogAnalyzer, shouldReturnLogsWithKeywordAnd) {
    QueryParser queryParser;
    const std::string q = R"(SOURCE = "AuthService" ORAZ LOG_LEVEL = "WARN")";
    const auto query = queryParser.parse(q);
    assertEntries(query, {false, false, false, false, true});
}

TEST(LogAnalyzer, shouldReturnLogsWithKeywordOr) {
    QueryParser queryParser;
    const std::string q = R"(MESSAGE = "User" LUB SOURCE = "API")";
    const auto query = queryParser.parse(q);
    assertEntries(query, {true, true, false, false, false});
}

TEST(LogAnalyzer, shouldReturnLogsForComplexQuery) {
    QueryParser queryParser;
    const std::string q = R"(LOG_LEVEL = "ERROR" ORAZ (Source = "AuthService" LUB message = "Transaction"))";
    const auto query = queryParser.parse(q);
    assertEntries(query, {false, false, false, false, false});
}

TEST(LogAnalyzer, shouldReturnLogsWithNotEquals) {
    QueryParser queryParser;
    const std::string q = R"(LOG_LEVEL != "INFO")";
    const auto query = queryParser.parse(q);
    assertEntries(query, {false, true, true, false, true});
}
