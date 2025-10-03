#pragma once

#include "GameObject.h"

class GameStateManager;

class EndScreenLogic : public BaseComponent
{
private:
    int score = 0;
    GameStateManager *stateManager = nullptr;
    GameObject *gameOverText = nullptr;
    GameObject *bg = nullptr;

public:
    EndScreenLogic();

    void Start() override;
    void Update(float deltaTime) override;
};
