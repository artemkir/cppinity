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
#include "TexturesManager.h"

EndScreenLogic::EndScreenLogic() {}

void EndScreenLogic::Start()
{
    auto scene = gameObject->GetScene();

    auto texture = scene->GetTextureManager()->CreateTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    // End Background
    bg = gameObject->CreateGameObjectBuilder("EndBackground", 0)
             .WithComponent<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE)
             .WithComponent<RectRenderer>(0, 0, 255)
             .AddToScene();

    // Game Over Text
    /*gameOverText = gameObject->CreateGameObjectBuilder("GameOverText", 0)
        .WithComponent<TileTransform>(WIDTH / 2 - 2, HEIGHT / 2 - 4, 32, 32)
        .WithComponent<SpriteRenderer>(texture)
        .AddToScene();*/

    stateManager = scene->FindGameObjectByName("StateMachineRoot")
                       ->GetComponent<GameStateManager>();
}

void EndScreenLogic::Update(float deltaTime)
{
    auto scene = gameObject->GetScene();
    auto &input = scene->GetInput();

    if (input.IsKeyPressed(Key::Space))
    {
        if (stateManager)
        {
            stateManager->TransitionTo(GameState::MainMenu);
        }
    }
}