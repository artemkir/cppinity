#include "Scripts/EndScreenLogic.h"

#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Scripts/GameConsts.h"
#include "Scripts/GameStateManager.h"
#include "Scripts/IconImage.h"
#include "Scene.h"
#include "GameObjectBuilder.h"

EndScreenLogic::EndScreenLogic(TexturesManager& textureManager_)
    : textureManager(textureManager_) {}

void EndScreenLogic::Start() {
    texture = textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    auto scene = gameObject->GetScene();

    // End Background
    scene->CreateGameObjectBuilder("EndBackground", 0)
        .WithComponent<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE)
        .WithComponent<RectRenderer>(20, 20, 20)
        .AddToScene();

    // Game Over Text
    scene->CreateGameObjectBuilder("GameOverText", 0)
        .WithComponent<TileTransform>(WIDTH / 2 - 2, HEIGHT / 2 - 4, 32, 32)
        .WithComponent<SpriteRenderer>(texture)
        .AddToScene();
}

void EndScreenLogic::Update(float deltaTime) {
    if (!gameObject) return; // Skip if deactivated

    auto scene = gameObject->GetScene();
    auto& input = scene->GetInput();

    if (input.IsKeyPressed(Key::Space))
    {
        auto stateManager = scene
            ->FindGameObjectByName("StateMachineRoot")
            ->GetComponent<GameStateManager>();

        if (stateManager)
        {
            stateManager->TransitionTo(GameState::MainMenu);
        }
    }
       
}