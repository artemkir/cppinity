#pragma once

#include "Std.h"
#include "Math/Vector2.h"
#include "BaseComponent.h"

class Animation : public BaseComponent
{
    float duration;           // Animation duration in seconds
    float elapsedTime = 0.0f; // Time elapsed since animation started
    Vector2 start;            // Starting scale
    Vector2 target;           // Target scale
    int loops;                // Number of loops, -1 for infinite loop

    float EaseInOutQuad(float t) const;

public:
    Animation(float duration_, Vector2 target_, int loops_ = 1);
    void Awake() override;
    void Update(float deltaTime) override;
};