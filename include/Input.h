#pragma once
#include <array>
#include <unordered_map>

#include <SDL2/SDL.h>

enum Key {
    A,
    D,
    W,
    S,
    R,
    X,
    Space,
    ESC,
    Count  // Sentinel for array size
};

class Input {
private:
    std::array<bool, static_cast<size_t>(Key::Count)> down;
    std::array<bool, static_cast<size_t>(Key::Count)> pressed;
    std::array<bool, static_cast<size_t>(Key::Count)> released;
    std::array<bool, static_cast<size_t>(Key::Count)> prevDown;

    std::unordered_map<SDL_Keycode, Key> keyMap;
    
    bool quitRequestedThisFrame;

public:
    Input();

    void Update(float deltaTime);

    bool IsKeyDown(Key key) const;
    bool IsKeyPressed(Key key) const;
    bool IsKeyReleased(Key key) const;

    bool WasQuitRequested() const;
};