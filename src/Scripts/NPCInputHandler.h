#pragma once

// NPC Input Handler Component
class NPCInputHandler : public IInputHandler {
private:
    Direction dir;
    std::vector<std::pair<int, int>> path;

    struct Node {
        int x, y;
        int g, h;
        Node* parent;

        Node(int x_, int y_, int g_, int h_, Node* p = nullptr) : x(x_), y(y_), g(g_), h(h_), parent(p) {}
        int f() const { return g + h; }
        bool operator>(const Node& other) const { return f() > other.f(); }
    };

    std::vector<std::pair<int, int>> FindPath(int startX, int startY, int goalX, int goalY) {
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
        std::set<std::pair<int, int>> closedList;
        openList.emplace(startX, startY, 0, std::abs(goalX - startX) + std::abs(goalY - startY));

        while (!openList.empty()) {
            Node current = openList.top();
            openList.pop();

            if (current.x == goalX && current.y == goalY) {
                std::vector<std::pair<int, int>> path;
                for (Node* node = &current; node != nullptr; node = node->parent) {
                    path.push_back({ node->x, node->y });
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            if (closedList.count({ current.x, current.y })) continue;
            closedList.insert({ current.x, current.y });

            std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
            for (auto [dx, dy] : directions) {
                int nx = current.x + dx;
                int ny = current.y + dy;

                if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) continue;

                bool isObstacle = false;
                for (const auto& go : gameObject->GetScene()->GetGameObjects()) {
                    if (go->GetTransform() &&
                        (go->GetName().find("maze_block_") == 0 || go->GetName().find("tail_") == 0) &&
                        go->GetTransform()->GetX() == nx &&
                        go->GetTransform()->GetY() == ny) {
                        isObstacle = true;
                        break;
                    }
                }
                if (isObstacle) continue;

                int g = current.g + 1;
                int h = std::abs(goalX - nx) + std::abs(goalY - ny);
                Node* parent = new Node(current.x, current.y, current.g, current.h, current.parent);
                openList.emplace(nx, ny, g, h, parent);
            }
        }
        return {};  // No path found
    }

public:
    NPCInputHandler() : dir(Direction::STOP) {}

    void Update(float deltaTime) override {
        if (!gameObject || !gameObject->GetTransform()) return;

        Transform* head = gameObject->GetTransform();
        GameObject* apple = nullptr;
        for (const auto& go : gameObject->GetScene()->GetGameObjects()) {
            if (go->GetName() == "apple") {
                apple = go.get();
                break;
            }
        }
        if (!apple || !apple->GetTransform()) return;

        int startX = head->GetX();
        int startY = head->GetY();
        int goalX = apple->GetTransform()->GetX();
        int goalY = apple->GetTransform()->GetY();

        path = FindPath(startX, startY, goalX, goalY);
        if (path.size() > 1) {
            int nextX = path[1].first;
            int nextY = path[1].second;
            if (nextX < startX) dir = Direction::LEFT;
            else if (nextX > startX) dir = Direction::RIGHT;
            else if (nextY < startY) dir = Direction::UP;
            else if (nextY > startY) dir = Direction::DOWN;
        }
        else {
            dir = Direction::STOP;
        }
    }

    Direction GetDirection() override { return dir; }
};