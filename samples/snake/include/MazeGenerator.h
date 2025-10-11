#pragma once

#include "Engine.h"

struct Vector2i
{
    int x;
    int y;

    bool operator<(const Vector2i& other) const {
        if (x != other.x)
        {
            return x < other.x;
        }

        return y < other.y;
    }
};

class MazeGenerator : public BaseComponent
{
    std::random_device rd;
    List<List<bool>> grid;

    void AddGridToScene();
    void GenerateMaze(int startX, int startY);
    
public:
    MazeGenerator() = default;
    bool IsObstacle(int x, int y);
    Vector2i GetRandomEmptyPosition();
    void Awake() override;
};