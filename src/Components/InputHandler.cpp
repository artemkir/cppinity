
#include "GameObject.h"
#include "Scene.h"
#include "Components/InputHandler.h"

InputHandler::InputHandler() : dir(Direction::STOP) {}

void InputHandler::Update(float deltaTime) {
   
    auto& input = gameObject->GetScene()->GetInput();

    if (input.IsKeyPressed(Key::ESC))
    {
        dir = Direction::STOP;
    } 

    if (input.IsKeyPressed(Key::A))
    {
        dir = Direction::LEFT;
    }
    else if (input.IsKeyPressed(Key::D))
    {
        dir = Direction::RIGHT;
    }
    else if (input.IsKeyPressed(Key::W))
    {
        dir = Direction::UP;
    }
    else if (input.IsKeyPressed(Key::S))
    {
        dir = Direction::DOWN;
    }
}

Direction InputHandler::GetDirection() {
    return dir;
}