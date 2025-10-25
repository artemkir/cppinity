#pragma once

#include "Engine.h"

class NPCInputHandler : public IInputHandler
{
    Direction dir;
    List<Vector2i> path;

    struct Node
    {
        int x, y;
        int g, h;
        SharedPtr<Node> parent;

        Node(int x_, int y_, int g_, int h_, SharedPtr<Node> parent_ = nullptr) : 
            x(x_), y(y_), g(g_), h(h_), parent(std::move(parent_)) {}
        int f() const { return g + h; }
        bool operator>(const Node &other) const { return f() > other.f(); }
    };

    struct NodeComparator
    {
        bool operator()(const SharedPtr<Node> &a, const SharedPtr<Node> &b) const
        {
            return a->f() > b->f();
        }
    };

    List<Vector2i> FindPath(int startX, int startY, int goalX, int goalY);

public:
    NPCInputHandler();

    void Update(float deltaTime) override;

    Direction GetDirection() override;
    void Stop() override;
};