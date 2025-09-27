#include "Components/InputHandler.h"
#include <SDL.h> 

InputHandler::InputHandler() : dir(Direction::STOP) {}

void InputHandler::Update(float deltaTime) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_x)) {
            dir = Direction::STOP;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_a: dir = Direction::LEFT; break;
                case SDLK_d: dir = Direction::RIGHT; break;
                case SDLK_w: dir = Direction::UP; break;
                case SDLK_s: dir = Direction::DOWN; break;
                default: break;
            }
        }
    }
}

Direction InputHandler::GetDirection() {
    return dir;
}