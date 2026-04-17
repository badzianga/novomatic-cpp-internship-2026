#ifndef BLUEPRINTAUTOLAYOUT_NODE_HPP
#define BLUEPRINTAUTOLAYOUT_NODE_HPP
#include <string>
#include <vector>

struct Node {
    int id = -1;
    std::string name;
    int x = 0;
    int y = 0;

    int depth = -1;

    std::vector<int> parents;
    std::vector<int> children;
};

#endif //BLUEPRINTAUTOLAYOUT_NODE_HPP
