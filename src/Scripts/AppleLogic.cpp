#include "Scripts/AppleLogic.h"
#include "GameObject.h"
#include "Scripts/MazeGenerator.h"
#include "Scripts/GameConsts.h"
#include "Components/Transform.h"

void AppleLogic::Awake() {
    mazeGenerator = gameObject->GetScene()->FindGameObjectByName("maze_generator")->GetComponent<MazeGenerator>();
    gameObject->GetTransform()->SetSize(TILE_SIZE, TILE_SIZE);
    RegenerateApple();
}

void AppleLogic::RegenerateApple() {
    auto [x, y] = mazeGenerator->GetRandomEmptyPosition();
    gameObject->GetTransform()->SetPosition(x, y);
}