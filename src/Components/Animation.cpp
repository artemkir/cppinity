#include <algorithm>
#include <cmath>

#include "Components/Animation.h"

#include "GameObject.h"
#include "Components/Transform.h"


Animation::Animation(float duration_, float targetX_, float targetY_, int loops_)
    : duration(duration_), targetX(targetX_), targetY(targetY_), loops(loops_) {
    if (duration_ <= 0.0f) {
        duration = 0.001f;
    }
}

float Animation::EaseInOutQuad(float t) const {
    t = std::max(0.0f, std::min(1.0f, t));
    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

void Animation::Start() {
    const TileTransform* transform = gameObject->GetTransform();
    startX = transform->GetScaleX();
    startY = transform->GetScaleY();
}

void Animation::Update(float deltaTime) {
    if (!active) return;

    TileTransform* transform = gameObject->GetTransform();
    elapsedTime += deltaTime;

    float t = elapsedTime / duration;
    if (t >= 1.0f) {

        loops--;

        if (loops == 0) {
            active = false;
            transform->SetScale(targetX, targetY);
            return;
        }

        std::swap(startX, targetX);
        std::swap(startY, targetY);
        elapsedTime = 0.0f;
        t = t - 1.0f;
    }

    float easedT = EaseInOutQuad(t);
    float newX = startX + (targetX - startX) * easedT;
    float newY = startY + (targetY - startY) * easedT;
    transform->SetScale(newX, newY);
}