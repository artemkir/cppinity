#include "Engine.h"

#include "Game.h"

NPCInputHandler::NPCInputHandler() : dir(Direction::STOP) {}

void NPCInputHandler::Update(float deltaTime)
{
    if (!gameObject || !gameObject->GetTransform())
        return;

    auto head = gameObject->GetTransform();
    GameObject *apple = nullptr;
    for (const auto &go : gameObject->GetScene()->GetGameObjects())
    {
        if (go->GetName() == "apple")
        {
            apple = go.get();
            break;
        }
    }
    if (!apple || !apple->GetTransform())
        return;

    int startX = head->GetX();
    int startY = head->GetY();
    int goalX = apple->GetTransform()->GetX();
    int goalY = apple->GetTransform()->GetY();

    path = FindPath(startX, startY, goalX, goalY);
    if (path.size() > 1)
    {
        int nextX = path[1].x;
        int nextY = path[1].y;
        if (nextX < startX)
            dir = Direction::LEFT;
        else if (nextX > startX)
            dir = Direction::RIGHT;
        else if (nextY < startY)
            dir = Direction::UP;
        else if (nextY > startY)
            dir = Direction::DOWN;
    }
    else
    {
        dir = Direction::STOP;
    }
}

Direction NPCInputHandler::GetDirection()
{
    return dir;
}

Vector<Vector2i> NPCInputHandler::FindPath(int startX, int startY, int goalX, int goalY)
{
    std::priority_queue<SharedPtr<Node>, Vector<SharedPtr<Node>>, NodeComparator> openList;
    Set<Vector2i> closedList;

    auto startNode = MakeShared<Node>(startX, startY, 0, std::abs(goalX - startX) + std::abs(goalY - startY));
    openList.push(startNode);

    while (!openList.empty())
    {
        auto current = openList.top();
        openList.pop();

        if (current->x == goalX && current->y == goalY)
        {
            Vector<Vector2i> path;
            for (auto node = current; node != nullptr; node = node->parent)
            {
                path.push_back({node->x, node->y});
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (closedList.count({current->x, current->y}))
            continue;

        closedList.insert({current->x, current->y});

        Vector<Vector2i> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (auto& [dx, dy] : directions)
        {
            int nx = current->x + dx;
            int ny = current->y + dy;

            if (nx < 0 || nx >= MAZE_WIDTH || ny < 0 || ny >= MAZE_HEIGHT)
                continue;

            bool isObstacle = false;
            for (const auto &go : gameObject->GetScene()->GetGameObjects())
            {
                if ((go->GetName().find("maze_block_") == 0 || go->GetName().find("tail_") == 0) &&
                    go->GetTransform()->GetX() == nx && go->GetTransform()->GetY() == ny)
                {
                    isObstacle = true;
                    break;
                }
            }
            if (isObstacle)
                continue;

            int g = current->g + 1;
            int h = std::abs(goalX - nx) + std::abs(goalY - ny);
            auto neighbor = MakeShared<Node>(nx, ny, g, h, current);
            openList.push(neighbor);
        }
    }

    return {};
}