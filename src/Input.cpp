#include "Input.h"

#include <SDL2/SDL.h>

Input::Input() {
    down.fill(false);
    pressed.fill(false);
    released.fill(false);
    prevDown.fill(false);

    // Initialize key mappings
    keyMap[SDLK_a] = Key::A;
    keyMap[SDLK_d] = Key::D;
    keyMap[SDLK_w] = Key::W;
    keyMap[SDLK_s] = Key::S;
    keyMap[SDLK_SPACE] = Key::Space;
    keyMap[SDLK_r] = Key::R;
    keyMap[SDLK_x] = Key::X;
    keyMap[SDLK_ESCAPE] = Key::X;
}

void Input::Update(float deltaTime) {
    quitRequestedThisFrame = false;
    pressed.fill(false);
    released.fill(false);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quitRequestedThisFrame = true;
        }
        else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            bool isDown = (event.type == SDL_KEYDOWN);
            
            auto it = keyMap.find(event.key.keysym.sym);
            if (it != keyMap.end()) {
                Key key = it->second;
                down[static_cast<size_t>(key)] = isDown;
            }
        }
    }

    // Compute pressed/released
    for (size_t i = 0; i < static_cast<size_t>(Key::Count); ++i) {
        pressed[i] = !prevDown[i] && down[i];
        released[i] = prevDown[i] && !down[i];
    }

    prevDown = down;
}

bool Input::IsKeyDown(Key key) const {
    return down[static_cast<size_t>(key)];
}

bool Input::IsKeyPressed(Key key) const {
    return pressed[static_cast<size_t>(key)];
}

bool Input::IsKeyReleased(Key key) const {
    return released[static_cast<size_t>(key)];
}

bool Input::WasQuitRequested() const {
    return quitRequestedThisFrame;
}