#pragma once

#include "BaseComponent.h"

class TileTransform;

class Animation : public BaseComponent
{
	float duration;			  // Animation duration in seconds
	float elapsedTime = 0.0f; // Time elapsed since animation started
	float startX = 0.0f;	  // Starting position
	float startY = 0.0f;
	float targetX; // Target position
	float targetY;
	int loops; // Number of loops, -1 infinite loop

	float EaseInOutQuad(float t) const;

public:
	Animation(float duration_, float targetX_, float targetY_, int loops_ = 1);
	void Awake() override;
	void Update(float deltaTime) override;
};
