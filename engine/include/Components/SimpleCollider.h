#pragma once

#include "BaseComponent.h"

class SimpleCollider : public BaseComponent
{
public:
	explicit SimpleCollider(bool active_ = true);
	bool IsColliding(const SimpleCollider &other) const;
};