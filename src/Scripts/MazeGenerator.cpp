#include "Scripts/MazeGenerator.h"

#include "Components/RectRenderer.h"
#include "Components/SimpleCollider.h"
#include "Scripts/GameConsts.h"
#include "Components/Transform.h"

void MazeGenerator::AddGridToScene() {
    std::mt19937 g(rd());
    std::uniform_int_distribution<> color(100, 140);

    // Add walls to Scene
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (!grid[y][x]) {
                int c = color(g);

                auto block = std::make_unique<GameObject>("maze_block_" + std::to_string(x) + "_" + std::to_string(y), OBSTACLE_TAG);
                block->AddComponent(std::make_unique<TileTransform>(x, y, TILE_SIZE, TILE_SIZE));
                block->AddComponent(std::make_unique<RectRenderer>(c, c, c));
                block->AddComponent(std::make_unique<SimpleCollider>());
                gameObject->GetScene()->AddGameObject(std::move(block));
            }
        }
    }
}

void MazeGenerator::GenerateMaze(int startX, int startY) {
    std::mt19937 g(rd());

    grid.resize(HEIGHT, std::vector<bool>(WIDTH, true));  // All walls initially
    std::stack<std::pair<int, int>> stack;
    stack.push({ startX, startY });
    grid[startY][startX] = false;  // Starting point is empty

    int _steps = 6;
    std::vector<std::pair<int, int>> directions = { {-_steps, 0}, {_steps, 0}, {0, -_steps}, {0, _steps} };

    while (!stack.empty()) {
        int x = stack.top().first;
        int y = stack.top().second;
        stack.pop();

        std::shuffle(directions.begin(), directions.end(), g);

        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && grid[ny][nx]) {
                int stepX = dx / _steps;
                int stepY = dy / _steps;
                int steps = _steps;

                for (int i = 1; i <= steps; ++i) {
                    int mx = x + stepX * i;
                    int my = y + stepY * i;
                    grid[my][mx] = false;
                }

                stack.push({ nx, ny });
            }
        }
    }

    // Random holes
    int numHoles = 20;
    std::uniform_int_distribution<> disX(0, WIDTH - 1);
    std::uniform_int_distribution<> disY(0, HEIGHT - 1);

    for (int i = 0; i < numHoles; ++i) {
        int attempts = 20;
        while (attempts-- > 0) {
            int hx = disX(g);
            int hy = disY(g);
            if (!grid[hy][hx]) {
                grid[hy][hx] = true;
                break;
            }
        }
    }

    AddGridToScene();
}

bool MazeGenerator::IsObstacle(int x, int y) {
    auto& sceneObjects = gameObject->GetScene()->GetGameObjects();

    for (const auto& go : sceneObjects) {
        if (go->GetTag() & OBSTACLE_TAG &&
            go->GetTransform()->GetX() == x &&
            go->GetTransform()->GetY() == y) {
            return true;
        }
    }

    return false;
}

std::pair<int, int> MazeGenerator::GetRandomEmptyPosition() {
    std::mt19937 g(rd());
    std::uniform_int_distribution<> disX(0, WIDTH - 1);
    std::uniform_int_distribution<> disY(0, HEIGHT - 1);

    int x = 0;
    int y = 0;

    const int MAX_ATTEMPTS = 1000;
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS) {
        attempts++;
        x = disX(g);
        y = disY(g);

        if (IsObstacle(x, y))
            continue;

        break;
    }

    return { x, y };
}

void MazeGenerator::Start() {
    GenerateMaze(WIDTH / 2, HEIGHT / 2);
}