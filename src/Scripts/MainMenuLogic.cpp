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
#include "GameObjectBuilder.h"  // Include the builder header

MainMenuLogic::MainMenuLogic(TexturesManager& textureManager_)
    : textureManager(textureManager_)
{
}

void MainMenuLogic::Start() {
    buttonTexture = textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    auto scene = gameObject->GetScene();

    // Menu Background
    bg = scene->CreateGameObjectBuilder("MenuBackground", 0)
        .WithComponent<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE)
        .WithComponent<RectRenderer>(50, 50, 50)
        .AddToScene();

    // Start Button
    startButton = scene->CreateGameObjectBuilder("StartButton", 0)
        .WithComponent<TileTransform>(WIDTH / 2 - 2, HEIGHT / 2 - 2, 32, 32)
        .WithComponent<SpriteRenderer>(buttonTexture)
        .AddToScene();

    auto stateManagerObject = scene->FindGameObjectByName("StateMachineRoot");
        
    if (stateManagerObject == nullptr)
    {
        SDL_Log("stateManagerObject == null");
    }

    stateManager = stateManagerObject->GetComponent<GameStateManager>();
}

void MainMenuLogic::OnActive(bool active) {
    bg->SetActive(active);
    startButton->SetActive(active);
}

void MainMenuLogic::Update(float deltaTime) {
    if (!gameObject) return; // Skip if deactivated

    auto scene = gameObject->GetScene();
    auto& input = scene->GetInput();

    if (input.IsKeyPressed(Key::Space))
    {
        if (stateManager)
        {
            stateManager->TransitionTo(GameState::GameMode);
        }
    }
}