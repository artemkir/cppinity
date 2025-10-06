#include "Components/SimpleCollider.h"
#include "GameObject.h"
#include "Components/Transform.h"

//#include "Scripts/GameConsts.h"

SimpleCollider::SimpleCollider(bool active_)
{
    SetActive(active_);
}

// TODO: Improve colliding
bool SimpleCollider::IsColliding(const SimpleCollider &other) const
{
    if (!active || !other.active || !gameObject || !other.gameObject)
        return false;

    const TileTransform *t1 = gameObject->GetTransform();
    const TileTransform *t2 = other.gameObject->GetTransform();

    if (!t1 || !t2)
        return false;

    int left1 = t1->GetX();
    int right1 = t1->GetX() + t1->GetWidth() / 40;
    int top1 = t1->GetY();
    int bottom1 = t1->GetY() + t1->GetHeight() / 40;

    int left2 = t2->GetX();
    int right2 = t2->GetX() + t2->GetWidth() / 40;
    int top2 = t2->GetY();
    int bottom2 = t2->GetY() + t2->GetHeight() / 40;

    return left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2;
}