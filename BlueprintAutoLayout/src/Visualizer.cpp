#include <ranges>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "Visualizer.hpp"

void Visualizer::openWindow(unsigned windowWidth, unsigned windowHeight, const std::string& title) {
    window.create(sf::VideoMode({ windowWidth, windowHeight }), title);
}

void Visualizer::loadFont(const std::string& filePath) {
    if (!font.openFromFile(filePath)) {
        throw std::runtime_error("Could not open font file: " + filePath);
    }
}

void Visualizer::loop(const Graph& graph) {
    bool dragging = false;
    sf::Vector2i lastMousePos;
    sf::View view = window.getView();

    while (window.isOpen()) {
        handleEvents(dragging, lastMousePos, view);

        window.clear(sf::Color::White);
        window.setView(view);

        drawEdges(graph);
        drawNodes(graph);

        window.display();
    }
}

void Visualizer::handleEvents(bool& dragging, sf::Vector2i& lastMousePos, sf::View& view) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();

        }
        else if (const auto* mouseScroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
            if (mouseScroll->delta > 0) {
                view.zoom(0.9f);
            }
            else {
                view.zoom(1.1f);
            }
        }
        else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePress->button == sf::Mouse::Button::Left) {
                dragging = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }
        }
        else if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseRelease->button == sf::Mouse::Button::Left) {
                dragging = false;
            }
        }
        else if (event->getIf<sf::Event::MouseMoved>() && dragging) {
            const sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
            const sf::Vector2f delta = window.mapPixelToCoords(lastMousePos) - window.mapPixelToCoords(newMousePos);

            view.move(delta);
            lastMousePos = newMousePos;
        }
    }
}

void Visualizer::drawEdges(const Graph& graph) {
    const auto& nodes = graph.getNodes();
    const float half = nodeSize.x / 2.f;

    for (const auto& node : nodes | std::views::values) {
        for (int childId : node.children) {
            const Node& child = nodes.at(childId);

            std::array line = {
                sf::Vertex(
                    sf::Vector2f(
                        static_cast<float>(node.x) + half,
                        static_cast<float>(node.y)),
                    sf::Color::Black),
                sf::Vertex(
                    sf::Vector2f(
                        static_cast<float>(child.x) - half,
                        static_cast<float>(child.y)),
                    sf::Color::Black
                )
            };

            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);

            sf::CircleShape circle(8);
            circle.setFillColor(sf::Color::Black);
            circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
            circle.setPosition(sf::Vector2f(static_cast<float>(child.x) - half, static_cast<float>(child.y)));

            window.draw(circle);
        }
    }
}

void Visualizer::drawNodes(const Graph& graph) {
    for (const auto& node : graph.getNodes() | std::views::values) {
        drawNode(node);
    }
}

void Visualizer::drawNode(const Node& node) {
    sf::RectangleShape rect(nodeSize);
    rect.setOrigin({ nodeSize.x / 2.f, nodeSize.y / 2.f });
    rect.setPosition({ static_cast<float>(node.x), static_cast<float>(node.y) });
    rect.setFillColor(sf::Color(200, 200, 250));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2);

    sf::Text text = makeWrappedText(node.name, 14, static_cast<int>(rect.getSize().x));

    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
    text.setPosition({ static_cast<float>(node.x), static_cast<float>(node.y) });

    window.draw(rect);
    window.draw(text);
}

sf::Text Visualizer::makeWrappedText(const std::string& str, unsigned charSize, int maxWidth) const {
    sf::Text text(font);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::Black);

    std::istringstream stream(str);
    std::string word;

    std::string line;
    std::string result;

    while (stream >> word) {
        std::string testLine = line + word + " ";
        text.setString(testLine);

        if (text.getLocalBounds().size.x > static_cast<float>(maxWidth) && !line.empty()) {
            result += line + "\n";
            line = word + " ";
        }
        else {
            line = testLine;
        }
    }

    result += line;
    text.setString(result);

    return text;
}
