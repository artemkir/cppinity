// Input.h
#pragma once
#include <array>
#include <unordered_map>

enum class Key
{
    A,
    D,
    W,
    S,
    R,
    X,
    Space,
    ESC,
    Count // Sentinel for array size
};

struct InputEvent;

class Input
{
private:
    std::array<bool, static_cast<size_t>(Key::Count)> down;
    std::array<bool, static_cast<size_t>(Key::Count)> pressed;
    std::array<bool, static_cast<size_t>(Key::Count)> released;
    std::array<bool, static_cast<size_t>(Key::Count)> prevDown;

    std::unordered_map<int, Key> keyMap;

    bool quitRequestedThisFrame = false;

public:
    Input();

    void BeginFrame();

    void HandleEvent(const InputEvent *event);

    bool IsKeyDown(Key key) const;
    bool IsKeyPressed(Key key) const;
    bool IsKeyReleased(Key key) const;

    bool WasQuitRequested() const;
};