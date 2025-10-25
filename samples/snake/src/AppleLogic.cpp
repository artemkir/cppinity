#include "Engine.h"

#include "Game.h"

void AppleLogic::Start()
{
    auto scene = gameObject->GetScene();
    auto canvas = scene->FindGameObjectByName("MainCanvas");
    auto canvasSize = canvas->GetComponent<Canvas>()->GetCanvasSize();

    Vector2 blockSize = canvasSize / Vector2{ MAZE_WIDTH, MAZE_HEIGHT };

    mazeGenerator = gameObject->GetScene()->FindGameObjectByName("maze_generator")->GetComponent<MazeGenerator>();
    gameObject->GetTransform()->SetSize(blockSize.x, blockSize.y);
    RegenerateApple();
}

void AppleLogic::RegenerateApple()
{
    auto [x, y] = mazeGenerator->GetRandomEmptyPosition();

    gameObject->GetTransform()->SetPosition(x, y);
}