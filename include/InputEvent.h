#pragma once

typedef enum EventType {
    EventType_KeyDown,
    EventType_KeyUp,
    EventType_QuitRequested,
  
    // Add more as needed (e.g., MouseMove, TouchBegin) for extensibility
    EventType_Unknown
} EventType;

typedef struct InputEvent {
    EventType type;
    int key_code;  // Equivalent to sapp_keycode
} InputEvent;