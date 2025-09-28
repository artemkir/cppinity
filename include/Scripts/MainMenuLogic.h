#pragma once

#include <memory>

#include "Components/BaseComponent.h"
#include "ITexture.h"

class GameObject;
class TexturesManager;
class GameStateManager;

class MainMenuLogic : public BaseComponent {
    TexturesManager& textureManager;
    std::shared_ptr<ITexture> buttonTexture;
    GameObject* bg = nullptr;
    GameObject* startButton = nullptr;
    GameStateManager* stateManager = nullptr;

public:
    MainMenuLogic(TexturesManager& textureManager_);

    void Start() override;
    void Update(float deltaTime) override;
};
