// Input.cpp (updated for Sokol)
#include "Input.h"
//#include "sokol_app.h"

Input::Input() {
    down.fill(false);
    pressed.fill(false);
    released.fill(false);
    prevDown.fill(false);

    // Initialize key mappings
    keyMap[SAPP_KEYCODE_A] = Key::A;
    keyMap[SAPP_KEYCODE_D] = Key::D;
    keyMap[SAPP_KEYCODE_W] = Key::W;
    keyMap[SAPP_KEYCODE_S] = Key::S;
    keyMap[SAPP_KEYCODE_SPACE] = Key::Space;
    keyMap[SAPP_KEYCODE_R] = Key::R;
    keyMap[SAPP_KEYCODE_X] = Key::X;
    keyMap[SAPP_KEYCODE_ESCAPE] = Key::ESC;
}

void Input::BeginFrame() {
    pressed.fill(false);
    released.fill(false);

    // Compute pressed/released (actual event handling in HandleEvent)
    for (size_t i = 0; i < static_cast<size_t>(Key::Count); ++i) {
        pressed[i] = !prevDown[i] && down[i];
        released[i] = prevDown[i] && !down[i];
    }

    prevDown = down;
    quitRequestedThisFrame = false;
}

void Input::HandleEvent(const sapp_event* e) {
    if (e->type == SAPP_EVENTTYPE_QUIT_REQUESTED) {
        quitRequestedThisFrame = true;
    } else if (e->type == SAPP_EVENTTYPE_KEY_DOWN || e->type == SAPP_EVENTTYPE_KEY_UP) {
        bool isDown = (e->type == SAPP_EVENTTYPE_KEY_DOWN);
        
        auto it = keyMap.find(e->key_code);
        if (it != keyMap.end()) {
            Key key = it->second;
            down[static_cast<size_t>(key)] = isDown;
        }
    }
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