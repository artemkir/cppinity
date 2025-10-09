#include "Engine.h"

#include "Game.h"

void MainMenuLogic::Start()
{
    auto scene = gameObject->GetScene();

    float screenW = (float)gameObject->GetScene()->GetRenderer()->GetW();
    float screenH = (float)gameObject->GetScene()->GetRenderer()->GetH();

    //auto buttonTexture = scene->GetTextureManager()->CreateTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    // Menu Background
    bg = gameObject->CreateGameObjectBuilder("MenuBackground", 0)
             .WithComponent<ScreenTransform>(0.0f, 0.0f, screenW, screenH)
             //.WithComponent<RectRenderer>(255, 0, 0)
             .WithComponent<SpriteRenderer>("start.png")
		     .WithComponent<Animation>(0.5f, 1.1f, 1.1f, -1)
             .AddToScene();

    // Start Button
    /*startButton = gameObject->CreateGameObjectBuilder("StartButton", 0)
        .WithComponent<GridTransform>(MAZE_WIDTH / 2 - 2, MAZE_HEIGHT / 2 - 2, 32, 32)
        .WithComponent<SpriteRenderer>(buttonTexture)
        .AddToScene();*/

    auto stateManagerObject = scene->FindGameObjectByName("StateMachineRoot");

    stateManager = stateManagerObject->GetComponent<GameStateManager>();
}

void MainMenuLogic::Update(float deltaTime)
{

    auto scene = gameObject->GetScene();
    auto &input = scene->GetInput();

    if (input.IsKeyPressed(Key::Space))
    {
        if (stateManager)
        {
            stateManager->TransitionTo(GameState::GameMode);
        }
    }
}