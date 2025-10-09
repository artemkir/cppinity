// Input.h
#pragma once

#include "Std.h"

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
    Array<bool, static_cast<size_t>(Key::Count)> down;
    Array<bool, static_cast<size_t>(Key::Count)> pressed;
    Array<bool, static_cast<size_t>(Key::Count)> released;
    Array<bool, static_cast<size_t>(Key::Count)> prevDown;

    UnorderedMap<int, Key> keyMap;

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