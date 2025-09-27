#pragma once

#include "Components/IInputHandler.h"
#include <vector>
#include <memory>
#include <set>

class NPCInputHandler : public IInputHandler {
    Direction dir;
    std::vector<std::pair<int, int>> path;

    struct Node {
        int x, y;
        int g, h;
        std::shared_ptr<Node> parent;

        Node(int x_, int y_, int g_, int h_, std::shared_ptr<Node> parent_ = nullptr) : x(x_), y(y_), g(g_), h(h_), parent(
            std::move(parent_)) {}
        int f() const { return g + h; }
        bool operator>(const Node& other) const { return f() > other.f(); }
    };

    struct NodeComparator {
        bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
            return a->f() > b->f();
        }
    };

    std::vector<std::pair<int, int>> FindPath(int startX, int startY, int goalX, int goalY);

public:
    NPCInputHandler();

    void Update(float deltaTime) override;

    Direction GetDirection() override;
};