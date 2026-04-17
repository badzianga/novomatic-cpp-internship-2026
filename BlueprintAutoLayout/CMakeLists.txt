cmake_minimum_required(VERSION 4.0)
project(BlueprintAutoLayout)

include(FetchContent)

FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git)
FetchContent_GetProperties(json)
if (NOT json_POPULATED)
    FetchContent_MakeAvailable(json)
endif()

FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git)
if (NOT SFML_POPULATED)
    FetchContent_MakeAvailable(SFML)
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(BlueprintAutoLayout main.cpp
        include/Node.hpp
        src/Graph.cpp
        include/Graph.hpp
        src/JsonReader.cpp
        include/JsonReader.hpp
        src/Visualizer.cpp
        include/Visualizer.hpp)
target_include_directories(BlueprintAutoLayout PRIVATE include)
target_link_libraries(BlueprintAutoLayout PRIVATE nlohmann_json::nlohmann_json sfml-graphics)

file(COPY ${CMAKE_SOURCE_DIR}/arial_narrow_7.ttf DESTINATION ${CMAKE_BINARY_DIR})
