#include "Scripts/MainMenuLogic.h"

#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Scripts/GameConsts.h"
#include "Scripts/GameStateManager.h"
#include "Scripts/IconImage.h"
#include "Scene.h"
#include "TexturesManager.h"

MainMenuLogic::MainMenuLogic(TexturesManager& textureManager_)
    : textureManager(textureManager_), bg(nullptr), startButton(nullptr)
{
}

void MainMenuLogic::Start() {
    buttonTexture = textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    auto bg = std::make_unique<GameObject>("MenuBackground");
    bg->AddComponent(std::make_unique<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
    bg->AddComponent(std::make_unique<RectRenderer>(50, 50, 50));
    this->bg = bg.get();
    gameObject->GetScene()->AddGameObject(std::move(bg));

    auto startButton = std::make_unique<GameObject>("StartButton");
    startButton->AddComponent(std::make_unique<TileTransform>(WIDTH / 2 - 2, HEIGHT / 2 - 2, 32, 32));
    startButton->AddComponent(std::make_unique<SpriteRenderer>(buttonTexture));
    this->startButton = startButton.get();
    gameObject->GetScene()->AddGameObject(std::move(startButton));
}

void MainMenuLogic::OnActive(bool active) {
    bg->SetActive(active);
    startButton->SetActive(active);
}

void MainMenuLogic::Update(float deltaTime) {
    if (!gameObject) return; // Skip if deactivated
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameObject->GetScene()->Stop();
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            auto stateManager = gameObject->GetScene()->FindGameObjectByName("StateMachineRoot")
                ->GetComponent<GameStateManager>();
            if (stateManager) stateManager->TransitionTo(GameState::GameMode);
        }
    }
}