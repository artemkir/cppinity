#include "Scripts/MainMenuLogic.h"

#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Scripts/GameConsts.h"
#include "Scripts/GameStateManager.h"
#include "Scripts/IconImage.h"
#include "Scene.h"
#include "GameObjectBuilder.h" // Include the builder header

void MainMenuLogic::Start()
{
    auto scene = gameObject->GetScene();

    //auto buttonTexture = scene->GetTextureManager()->CreateTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    // Menu Background
    bg = gameObject->CreateGameObjectBuilder("MenuBackground", 0)
             .WithComponent<TileTransform>(0, 0, WIDTH * TILE_SIZE / 2, HEIGHT * TILE_SIZE / 2)
             //.WithComponent<RectRenderer>(255, 0, 0)
             .WithComponent<SpriteRenderer>("girl.png")
             .AddToScene();

    // Start Button
    /*startButton = gameObject->CreateGameObjectBuilder("StartButton", 0)
        .WithComponent<TileTransform>(WIDTH / 2 - 2, HEIGHT / 2 - 2, 32, 32)
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