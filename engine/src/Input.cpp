// Input.cpp
#include "Input.h"
#include "InputEvent.h"

Input::Input()
{
    down.fill(false);
    pressed.fill(false);
    released.fill(false);
    prevDown.fill(false);

    // Check SAPP_KEYCODE_A, etc. for correct values
    keyMap[65] = Key::A;
    keyMap[68] = Key::D;
    keyMap[87] = Key::W;
    keyMap[83] = Key::S;
    keyMap[32] = Key::Space;
    keyMap[82] = Key::R;
    keyMap[88] = Key::X;
    keyMap[256] = Key::ESC;
}

void Input::BeginFrame()
{
    pressed.fill(false);
    released.fill(false);

    // Compute pressed/released (actual event handling in HandleEvent)
    for (size_t i = 0; i < static_cast<size_t>(Key::Count); ++i)
    {
        pressed[i] = !prevDown[i] && down[i];
        released[i] = prevDown[i] && !down[i];
    }

    prevDown = down;
    quitRequestedThisFrame = false;
}

void Input::HandleEvent(const InputEvent *event)
{
    if (event->type == EventType_QuitRequested)
    {
        quitRequestedThisFrame = true;
    }
    else if (event->type == EventType_KeyDown || event->type == EventType_KeyUp)
    {
        bool isDown = (event->type == EventType_KeyDown);

        auto it = keyMap.find(event->key_code);
        if (it != keyMap.end())
        {
            Key key = it->second;
            down[static_cast<size_t>(key)] = isDown;
        }
    }
}

bool Input::IsKeyDown(Key key) const
{
    return down[static_cast<size_t>(key)];
}

bool Input::IsKeyPressed(Key key) const
{
    return pressed[static_cast<size_t>(key)];
}

bool Input::IsKeyReleased(Key key) const
{
    return released[static_cast<size_t>(key)];
}

bool Input::WasQuitRequested() const
{
    return quitRequestedThisFrame;
}