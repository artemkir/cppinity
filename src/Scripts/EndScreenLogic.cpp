#include "Scripts/EndScreenLogic.h"

#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Scripts/GameConsts.h"
#include "Scripts/GameStateManager.h"
#include "Scripts/IconImage.h"
#include "Scene.h"

EndScreenLogic::EndScreenLogic(TexturesManager& textureManager_)
    : textureManager(textureManager_) {}

void EndScreenLogic::Start() {
    texture = textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    auto bg = std::make_unique<GameObject>("EndBackground");
    bg->AddComponent(std::make_unique<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
    bg->AddComponent(std::make_unique<RectRenderer>(20, 20, 20));
    gameObject->GetScene()->AddGameObject(std::move(bg));

    auto gameOver = std::make_unique<GameObject>("GameOverText");
    gameOver->AddComponent(std::make_unique<TileTransform>(WIDTH / 2 - 2, HEIGHT / 2 - 4, 32, 32));
    gameOver->AddComponent(std::make_unique<SpriteRenderer>(texture));
    gameObject->GetScene()->AddGameObject(std::move(gameOver));
}

void EndScreenLogic::Update(float deltaTime) {
    if (!gameObject) return; // Skip if deactivated

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)) {
            auto stateManager = gameObject->GetScene()->FindGameObjectByName("StateMachineRoot")
                ->GetComponent<GameStateManager>();
            if (stateManager) stateManager->TransitionTo(GameState::MainMenu);
        }
    }
}