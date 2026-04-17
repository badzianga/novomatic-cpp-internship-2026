#ifndef BLUEPRINTAUTOLAYOUT_GRAPH_HPP
#define BLUEPRINTAUTOLAYOUT_GRAPH_HPP
#include <map>
#include <nlohmann/json_fwd.hpp>

#include "Node.hpp"

class Graph {
public:
    void loadFromJson(const nlohmann::json& json);
    void autoLayout(int xSpacing, int ySpacing);
    [[nodiscard]] const std::map<int, Node>& getNodes() const;
private:
    void initVirtualRoot();
    void assignDepths();
    void groupLayers();
    void assignXs(int xSpacing);
    void assignYs(int ySpacing);

    std::map<int, Node> nodes;
    std::map<int, std::vector<int>> layers;
    Node virtualRoot;
};

#endif //BLUEPRINTAUTOLAYOUT_GRAPH_HPP
