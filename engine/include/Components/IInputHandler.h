#pragma once

#include "Components/BaseComponent.h"

enum class Direction
{
    STOP,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class IInputHandler : public BaseComponent
{
public:
    virtual Direction GetDirection() = 0;
};
