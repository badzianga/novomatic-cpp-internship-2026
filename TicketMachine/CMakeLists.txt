cmake_minimum_required(VERSION 4.0)
project(TicketMachine)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(TicketMachineLib
        src/CashRegister.cpp
        include/CashRegister.hpp
        include/Coin.hpp
        src/Coin.cpp
        src/TicketMachineClient.cpp
        include/TicketMachineClient.hpp
        src/TicketMachineServer.cpp
        include/TicketMachineServer.hpp
        src/UI.cpp
        include/UI.hpp
        include/Ticket.hpp
        include/Reservation.hpp)
target_include_directories(TicketMachineLib PUBLIC include)

add_executable(TicketMachine main.cpp)
target_link_libraries(TicketMachine PRIVATE TicketMachineLib)

add_executable(TicketMachineTests test/test.cpp)
target_link_libraries(TicketMachineTests PRIVATE TicketMachineLib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TicketMachineTests)
