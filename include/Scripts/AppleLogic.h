#pragma once

#include "Components/BaseComponent.h"

class MazeGenerator;

class AppleLogic : public BaseComponent {
    MazeGenerator* mazeGenerator = nullptr;

public:
    void Awake() override;
    void RegenerateApple();
};
