#pragma once

#include "GameObject.h"
#include "TexturesManager.h"

class EndScreenLogic : public BaseComponent {
private:
    TexturesManager& textureManager;
    int score = 0;
    std::shared_ptr<ITexture> texture;

public:
    EndScreenLogic(TexturesManager& textureManager_);

    void Start() override;
    void Update(float deltaTime) override;
};
