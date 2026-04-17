#include <nlohmann/json.hpp>
#include <queue>
#include "Graph.hpp"

void Graph::loadFromJson(const nlohmann::json& json) {
    nodes.clear();

    for (auto& nodeJson : json["nodes"]) {
        const int id = nodeJson["id"].get<int>();
        nodes[id] = Node(
            id,
            nodeJson["name"].get<std::string>(),
            nodeJson["x"].get<int>(),
            nodeJson["y"].get<int>()
        );
    }

    for (auto& edgeJson : json["edges"]) {
        const int from = edgeJson["from"].get<int>();
        const int to = edgeJson["to"].get<int>();

        nodes[from].children.push_back(to);
        nodes[to].parents.push_back(from);
    }
}

void Graph::autoLayout(int xSpacing, int ySpacing) {
    initVirtualRoot();
    assignDepths();
    groupLayers();

    assignYs(ySpacing);
    for (int i = 0; i < 10; ++i) {
        assignYs(150);
    }
    assignXs(xSpacing);
}

const std::map<int, Node>& Graph::getNodes() const {
    return nodes;
}

void Graph::initVirtualRoot() {
    for (const auto& [id, node] : nodes) {
        if (node.parents.empty()) {
            virtualRoot.children.push_back(id);
        }
    }
}

void Graph::assignDepths() {
    std::queue<int> q;

    for (int r : virtualRoot.children) {
        nodes[r].depth = 0;
        q.push(r);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        const Node& uNode = nodes[u];

        for (int v : uNode.children) {
            const int newDepth = uNode.depth + 1;
            Node& vNode = nodes[v];

            if (newDepth > vNode.depth) {
                vNode.depth = newDepth;
                q.push(v);
            }
        }
    }
}

void Graph::groupLayers() {
    layers.clear();

    for (const auto& [id, node] : nodes) {
        layers[node.depth].push_back(id);
    }
}

void Graph::assignXs(int xSpacing) {
    for (auto& node : nodes | std::views::values) {
        node.x = node.depth * xSpacing;
    }
}

void Graph::assignYs(int ySpacing) {
    for (auto& layer : layers | std::views::values) {
        for (int nodeId : layer) {
            auto& node = nodes[nodeId];

            if (!node.parents.empty()) {
                double sum = 0;
                for (int p : node.parents) {
                    sum += nodes[p].y;
                }
                node.y = sum / node.parents.size();
            }
        }

        // resolve overlap
        for (int i = 1; i < layer.size(); ++i) {
            const auto& left = nodes[layer[i - 1]];
            auto& right = nodes[layer[i]];

            if (right.y - left.y < ySpacing) {
                right.y = left.y + ySpacing;
            }
        }
    }
}

