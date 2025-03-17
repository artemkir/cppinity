#pragma once

#include "Components/BaseComponent.h"

class MazeGenerator;

class AppleLogic : public BaseComponent {
    MazeGenerator* mazeGenerator = nullptr;

public:
    void Start() override;
    void RegenerateApple();
};
