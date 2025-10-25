#include "Components/Animation.h"
#include "GameObject.h"
#include "Components/Transform.h"

Animation::Animation(float duration_, Vector2 target_, int loops_)
    : duration(duration_), target(target_), loops(loops_)
{
    if (duration_ <= 0.0f)
    {
        duration = 0.001f;
    }
}

float Animation::EaseInOutQuad(float t) const
{
    t = std::max(0.0f, std::min(1.0f, t));
    return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

void Animation::Awake()
{
    auto transform = gameObject->GetTransform();
    start = transform->GetScale();
}

void Animation::Update(float deltaTime)
{
    if (!active)
        return;

    auto transform = gameObject->GetTransform();
    elapsedTime += deltaTime;

    float t = elapsedTime / duration;
    if (t >= 1.0f)
    {
        loops--;

        if (loops == 0)
        {
            active = false;
            transform->SetScale(target.x, target.y);
            return;
        }

        std::swap(start, target);
        elapsedTime = 0.0f;
        t = t - 1.0f;
    }

    float easedT = EaseInOutQuad(t);
    Vector2 newScale = start + (target - start) * easedT;
    transform->SetScale(newScale.x, newScale.y);
}