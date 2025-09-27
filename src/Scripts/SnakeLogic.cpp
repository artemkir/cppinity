
#include "Scripts/SnakeLogic.h"

#include "Scripts/MazeGenerator.h"
#include "Scripts/AppleLogic.h"
#include "Components/RectRenderer.h"
#include "Components/SimpleCollider.h"
#include "Components/Transform.h"
#include "Components/IInputHandler.h"
#include "Scene.h"
#include "Scripts/GameStateManager.h"
#include "GameObject.h"
#include <memory>

#include "Scripts/GameConsts.h"

void SnakeLogic::Start() {
    inputHandler = gameObject->GetComponent<IInputHandler>();
    mazeGenerator = gameObject->GetScene()->FindGameObjectByName("maze_generator")->GetComponent<MazeGenerator>();
    apple = gameObject->GetScene()->FindGameObjectByName("apple")->GetComponent<AppleLogic>();
    rect = gameObject->GetComponent<RectRenderer>();

    auto [x, y] = mazeGenerator->GetRandomEmptyPosition();
    gameObject->GetTransform()->SetPosition(x, y);
    gameObject->GetTransform()->SetSize(TILE_SIZE, TILE_SIZE);
}

void SnakeLogic::Update(float deltaTime) {
    updateTimer += deltaTime;

    if (updateTimer < update_interval) return;

    updateTimer -= update_interval;

    auto dir = inputHandler->GetDirection();
    TileTransform* headTransform = gameObject->GetTransform();

    if (dir == Direction::STOP) return;

    int prevX = headTransform->GetX();
    int prevY = headTransform->GetY();

    switch (dir) {
    case Direction::LEFT: headTransform->SetPosition(prevX - 1, prevY); break;
    case Direction::RIGHT: headTransform->SetPosition(prevX + 1, prevY); break;
    case Direction::UP: headTransform->SetPosition(prevX, prevY - 1); break;
    case Direction::DOWN: headTransform->SetPosition(prevX, prevY + 1); break;
    default: break;
    }

    // Wrap around logic
    if (headTransform->GetX() < 0) headTransform->SetPosition(WIDTH - 1, headTransform->GetY());
    if (headTransform->GetX() >= WIDTH) headTransform->SetPosition(0, headTransform->GetY());
    if (headTransform->GetY() < 0) headTransform->SetPosition(headTransform->GetX(), HEIGHT - 1);
    if (headTransform->GetY() >= HEIGHT) headTransform->SetPosition(headTransform->GetX(), 0);

    for (size_t i = 0; i < tail.size(); i++) {
        TileTransform* tailTransform = tail[i]->GetTransform();
        int tempX = tailTransform->GetX();
        int tempY = tailTransform->GetY();
        tailTransform->SetPosition(prevX, prevY);
        prevX = tempX;
        prevY = tempY;

        tail[i]->GetComponent<SimpleCollider>()->SetActive(true);
    }
}

void SnakeLogic::OnCollide(GameObject* other) {
    if (!other) return;

    if (other->GetName() == "apple") {
        score += 10;

        apple->RegenerateApple();

        TileTransform* headTransform = gameObject->GetTransform();
        int prevX = headTransform->GetX();
        int prevY = headTransform->GetY();
        if (!tail.empty()) {
            prevX = tail.back()->GetTransform()->GetX();
            prevY = tail.back()->GetTransform()->GetY();
        }
        auto newTail = std::make_unique<GameObject>("tail_" + std::to_string(tail.size()), OBSTACLE_TAG);

        auto color = rect->GetColor();

        newTail->AddComponent(std::make_unique<RectRenderer>((int)(color.r * 0.75f), (int)(color.g * 0.75f), (int)(color.b * 0.75f)));
        newTail->AddComponent(std::make_unique<TileTransform>(prevX, prevY, TILE_SIZE, TILE_SIZE));
        newTail->AddComponent(std::make_unique<SimpleCollider>(false));
        tail.push_back(newTail.get());

        if (tail.size() > 0 && update_interval > min_update_interval) {
            update_interval -= tail_change_interval;
        }

        gameObject->GetScene()->AddGameObject(std::move(newTail));
    } else if (other->GetTag() & OBSTACLE_TAG) { 
        auto stateManager = gameObject->GetScene()->FindGameObjectByName("StateMachineRoot")
            ->GetComponent<GameStateManager>();
        if (stateManager) {
            //stateManager->SetFinalScore(score);
            stateManager->TransitionTo(GameState::EndScreen);
        }
    }
}

int SnakeLogic::GetScore() { return score; }