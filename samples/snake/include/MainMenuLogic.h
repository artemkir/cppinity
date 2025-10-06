#pragma once

#include "Engine.h"

class MainMenuLogic : public BaseComponent
{
    GameObject *bg = nullptr;
    GameObject *startButton = nullptr;
    GameStateManager *stateManager = nullptr;

public:
    MainMenuLogic() = default;

    void Start() override;
    void Update(float deltaTime) override;
};
