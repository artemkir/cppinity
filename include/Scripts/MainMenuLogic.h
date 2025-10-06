#pragma once

#include "Components/BaseComponent.h"

class GameObject;
class GameStateManager;

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
