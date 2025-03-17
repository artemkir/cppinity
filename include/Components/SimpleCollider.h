#pragma once

#include "BaseComponent.h"

class SimpleCollider : public BaseComponent {
public:
	SimpleCollider(bool active_ = true);
	bool IsColliding(const SimpleCollider& other) const;
};