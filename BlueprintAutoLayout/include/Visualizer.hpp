#ifndef BLUEPRINTAUTOLAYOUT_VISUALIZER_HPP
#define BLUEPRINTAUTOLAYOUT_VISUALIZER_HPP
#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Graph.hpp"

class Visualizer {
public:
    void openWindow(unsigned windowWidth, unsigned windowHeight, const std::string& title);
    void loadFont(const std::string& filePath);
    void loop(const Graph& graph);
private:
    void handleEvents(bool& dragging, sf::Vector2i& lastMousePos, sf::View& view);
    void drawEdges(const Graph& graph);
    void drawNodes(const Graph& graph);
    void drawNode(const Node& node);
    sf::Text makeWrappedText(const std::string& str, unsigned charSize, int maxWidth) const;

    sf::Vector2f nodeSize = { 150.f, 90.f };
    sf::RenderWindow window;
    sf::Font font;
};

#endif //BLUEPRINTAUTOLAYOUT_VISUALIZER_HPP
