#include "Engine.h"

#include "Game.h"

void EndScreenLogic::Start()
{
    auto scene = gameObject->GetScene();

    //auto texture = scene->GetTextureManager()->CreateTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    // End Background
    bg = gameObject->CreateGameObjectBuilder("EndBackground", 0)
             .WithComponent<GridTransform>(0, 0, MAZE_WIDTH * TILE_SIZE, MAZE_HEIGHT * TILE_SIZE)
             .WithComponent<RectRenderer>(0, 0, 255)
             .AddToScene();

    // Game Over Text
    /*gameOverText = gameObject->CreateGameObjectBuilder("GameOverText", 0)
        .WithComponent<GridTransform>(MAZE_WIDTH / 2 - 2, MAZE_HEIGHT / 2 - 4, 32, 32)
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