#include "Engine.h"

#include "Game.h"

void EndScreenLogic::Start()
{
    auto scene = gameObject->GetScene();

    float screenW = (float)gameObject->GetScene()->GetRenderer()->GetW();
    float screenH = (float)gameObject->GetScene()->GetRenderer()->GetH();

    // End Background
    bg = gameObject->CreateGameObjectBuilder("EndBackground", 0)
             .WithComponent<ScreenTransform>(0, 0, screenW, screenH)
             .WithComponent<SpriteRenderer>("lose.png")
             .WithComponent<Animation>(0.5f, 1.1f, 1.1f, -1)
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