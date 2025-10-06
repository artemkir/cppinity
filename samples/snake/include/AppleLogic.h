#pragma once

#include "Engine.h"

class MazeGenerator;

class AppleLogic : public BaseComponent
{
    MazeGenerator *mazeGenerator = nullptr;

public:
    void Awake() override;
    void RegenerateApple();
};
