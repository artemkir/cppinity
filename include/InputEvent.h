#pragma once

typedef enum EventType
{
    EventType_KeyDown,
    EventType_KeyUp,
    EventType_QuitRequested,
    EventType_Unknown
} EventType;

typedef struct InputEvent
{
    EventType type;
    int key_code;
} InputEvent;