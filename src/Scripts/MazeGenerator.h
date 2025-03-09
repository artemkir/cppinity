#pragma once

#include "Scene.h"
#include <stack>
#include <algorithm>
#include <iterator>
#include <random>

class MazeGenerator : public BaseComponent {

    std::random_device rd;
    std::vector<std::vector<bool>> grid;

    void AddGridToScene()
    {
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

    void GenerateMaze(int startX, int startY) {
        
        std::mt19937 g(rd());

        grid.resize(HEIGHT, std::vector<bool>(WIDTH, true));  // All walls initially
        std::stack<std::pair<int, int>> stack;
        stack.push({ startX, startY });
        grid[startY][startX] = false;  // Starting point is empty

        int _steps = 6;

        std::vector<std::pair<int, int>> directions = { {-_steps, 0}, {_steps, 0}, {0, -_steps}, {0, _steps} };  // 4-unit steps for big spaces

        while (!stack.empty()) {
            int x = stack.top().first;
            int y = stack.top().second;
            stack.pop();

            std::shuffle(directions.begin(), directions.end(), g);

            for (auto [dx, dy] : directions) {
                int nx = x + dx;
                int ny = y + dy;

                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && grid[ny][nx]) {
                    // Carve a full path from (x, y) to (nx, ny)
                    int stepX = dx / _steps;  // Step size in x direction (1 or -1)
                    int stepY = dy / _steps;  // Step size in y direction (1 or -1)
                    int steps = _steps;       // Number of steps (based on direction magnitude)

                    for (int i = 1; i <= steps; ++i) {
                        int mx = x + stepX * i;
                        int my = y + stepY * i;
                        grid[my][mx] = false;  // Clear all cells along the path
                    }

                    stack.push({ nx, ny });
                }
            }
        }
        
        //random holes
        int numHoles = 20;  // Number of holes to add (adjustable)
        std::uniform_int_distribution<> disX(0, WIDTH - 1);
        std::uniform_int_distribution<> disY(0, HEIGHT - 1);

        for (int i = 0; i < numHoles; ++i) {
            int attempts = 20;  // Prevent infinite loops
            while (attempts-- > 0) {
                int hx = disX(g);
                int hy = disY(g);
                if (!grid[hy][hx]) {  // If it’s a wall
                    grid[hy][hx] = true;  // Create a hole
                    break;
                }
            }
        }
                

        AddGridToScene();
    }

    //unused
    bool __IsReachable(int x, int y, int startX, int startY) {
        std::vector<std::vector<bool>> visited(HEIGHT, std::vector<bool>(WIDTH, false));
        std::queue<std::pair<int, int>> q;
        q.push({ startX, startY });
        visited[startY][startX] = true;

        std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

        while (!q.empty()) {
            auto [cx, cy] = q.front();
            q.pop();

            if (cx == x && cy == y) return true;

            for (auto [dx, dy] : directions) {
                int nx = cx + dx;
                int ny = cy + dy;

                if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT || visited[ny][nx]) continue;

                bool isObstacle = false;
                for (const auto& go : gameObject->GetScene()->GetGameObjects()) {
                    if (go->GetTransform() &&
                        (go->GetName().find("MazeBlock_") == 0 || go->GetName().find("Tail_") == 0) &&
                        go->GetTransform()->GetX() == nx &&
                        go->GetTransform()->GetY() == ny) {
                        isObstacle = true;
                        break;
                    }
                }
                if (!isObstacle) {
                    visited[ny][nx] = true;
                    q.push({ nx, ny });
                }
            }
        }
        return false;
    }
       

public:
    MazeGenerator() {}

    bool IsObstacle(int x, int y)
    {
        auto& sceneObjects = gameObject->GetScene()->GetGameObjects();

        bool isObstacle = false;
        for (const auto& go : sceneObjects) {
            if (go->GetTag() & OBSTACLE_TAG &&
                go->GetTransform()->GetX() == x &&
                go->GetTransform()->GetY() == y) {
                return true;
            }
        }

        return false;
    }

    std::pair<int,int> GetRandomEmptyPosition() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<> disX(0, WIDTH - 1);
        std::uniform_int_distribution<> disY(0, HEIGHT - 1);

        int x = 0;
        int y = 0;

        const int MAX_ATTEMPTS = 1000; // Prevent infinite loops
        int attempts = 0;

        while (attempts < MAX_ATTEMPTS) {
            attempts++;

            x = disX(g);
            y = disY(g);

            if (IsObstacle(x, y))
                continue;
                        
            break;
        }

        return { x,y };
    }

    void Start() override {
        GenerateMaze(WIDTH / 2, HEIGHT / 2);
    }
};