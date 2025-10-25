#pragma once

#include "Engine.h"

class GameStateManager;

class EndScreenLogic : public BaseComponent
{
private:
    int score = 0;
    GameStateManager *stateManager = nullptr;
    GameObject *gameOverText = nullptr;
    GameObject *bg = nullptr;

public:
    EndScreenLogic() = default;

    void Start() override;
    void Update(float deltaTime) override;
	void OnActive(bool isActive) override;
};
