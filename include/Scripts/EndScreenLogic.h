#pragma once

#include "GameObject.h"
#include "TexturesManager.h"

class GameStateManager;

class EndScreenLogic : public BaseComponent
{
private:
    TexturesManager &textureManager;
    int score = 0;
    std::shared_ptr<ITexture> texture;
    GameStateManager *stateManager = nullptr;
    GameObject *gameOverText = nullptr;
    GameObject *bg = nullptr;

public:
    EndScreenLogic(TexturesManager &textureManager_);

    void Start() override;
    void Update(float deltaTime) override;
};
