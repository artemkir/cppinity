#pragma once

#include "Engine.h"

class MazeGenerator : public BaseComponent
{
    std::random_device rd;
    Vector<Vector<bool>> grid;

    void AddGridToScene();
    void GenerateMaze(int startX, int startY);
    bool __IsReachable(int x, int y, int startX, int startY);

public:
    MazeGenerator() = default;
    bool IsObstacle(int x, int y);
    std::pair<int, int> GetRandomEmptyPosition();
    void Awake() override;
};