#include "Components/SimpleCollider.h"
#include "GameObject.h"
#include "Components/Transform.h"

SimpleCollider::SimpleCollider(bool active_)
{
    SetActive(active_);
}

// TODO: Improve colliding
bool SimpleCollider::IsColliding(const SimpleCollider &other) const
{
    if (!active || !other.active || !gameObject || !other.gameObject)
        return false;

    auto *t1 = gameObject->GetTransform();
    auto *t2 = other.gameObject->GetTransform();

    if (!t1 || !t2)
        return false;

    auto s1 = t1->GetScreenTransform();
    auto s2 = t2->GetScreenTransform();

    int left1 = s1.pos.x;
    int right1 = s1.pos.x + s1.size.x;
    int top1 = s1.pos.y;
    int bottom1 = s1.pos.y + s1.size.y;

    int left2 = s2.pos.x;
    int right2 = s2.pos.x + s2.size.x;
    int top2 = s2.pos.y;
    int bottom2 = s2.pos.y + s2.size.y;

    return left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2;
}