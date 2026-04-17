#include <iostream>
#include <nlohmann/json.hpp>
#include "Graph.hpp"
#include "JsonReader.hpp"
#include "Visualizer.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " filename" << std::endl;
        return 0;
    }

    Graph graph;

    try {
        const auto json = JsonReader::readFromFile(argv[1]);
        graph.loadFromJson(json);
        graph.autoLayout(200, 150);

        Visualizer visualizer;
        visualizer.openWindow(1280, 720, "Blueprint Auto Layout");
        visualizer.loadFont("arial_narrow_7.ttf");
        visualizer.loop(graph);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
