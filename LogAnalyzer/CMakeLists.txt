cmake_minimum_required(VERSION 4.0)
project(LogAnalyzer)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(LogAnalyzerLib
        include/LogEntry.hpp
        include/Token.hpp
        src/Lexer.cpp
        include/Lexer.hpp
        src/Query.cpp
        include/Query.hpp
        src/FileParser.cpp
        include/FileParser.hpp
        src/QueryParser.cpp
        include/QueryParser.hpp
        src/Parser.cpp
        include/Parser.hpp
        src/LogEntry.cpp)
target_include_directories(LogAnalyzerLib PUBLIC include)

add_executable(LogAnalyzer main.cpp)
target_link_libraries(LogAnalyzer PRIVATE LogAnalyzerLib)

add_executable(LogAnalyzerTests test/test.cpp)
target_link_libraries(LogAnalyzerTests PRIVATE LogAnalyzerLib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(LogAnalyzerTests)
