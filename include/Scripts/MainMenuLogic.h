#pragma once

#include <memory>

#include "Components/BaseComponent.h"
#include "ITexture.h"

class GameObject;
class TexturesManager;

class MainMenuLogic : public BaseComponent {
    TexturesManager& textureManager;
    std::shared_ptr<ITexture> buttonTexture;
    GameObject* bg;
    GameObject* startButton;
    GameStateManager* stateManager;

public:
    MainMenuLogic(TexturesManager& textureManager_);

    void Start() override;
    void OnActive(bool active) override;
    void Update(float deltaTime) override;
};
