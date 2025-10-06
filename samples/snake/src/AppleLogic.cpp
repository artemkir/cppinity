#include "AppleLogic.h"
#include "GameObject.h"
#include "MazeGenerator.h"
#include "GameConsts.h"
#include "Components/Transform.h"

void AppleLogic::Awake()
{
    float blockSizeW = (float)gameObject->GetScene()->GetRenderer()->GetW() / MAZE_WIDTH;
    float blockSizeH = (float)gameObject->GetScene()->GetRenderer()->GetH() / MAZE_HEIGHT;

    mazeGenerator = gameObject->GetScene()->FindGameObjectByName("maze_generator")->GetComponent<MazeGenerator>();
    gameObject->GetTransform()->SetSize(blockSizeW, blockSizeH);
    RegenerateApple();
}

void AppleLogic::RegenerateApple()
{
    auto [x, y] = mazeGenerator->GetRandomEmptyPosition();

    gameObject->GetTransform()->SetPosition(x, y);
}