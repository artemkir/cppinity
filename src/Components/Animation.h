#pragma once

// Animation Controller Component
class Animation : public BaseComponent {
private:
	float duration;			// Animation duration in seconds
	float elapsedTime;		// Time elapsed since animation started
	float startX, startY;   // Starting position
	float targetX, targetY; // Target position
	int loops;              // Number of loops, -1 infinite loop
	bool active;            // Whether the animation is running

	float EaseInOutQuad(float t) {
		t = std::max(0.0f, std::min(1.0f, t));  // Clamp t between 0 and 1
		return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

public:
	Animation(float duration_, float targetX_, float targetY_, int loops_ = 1)
		:
		duration(duration_),
		elapsedTime(0.0f),
		targetX(targetX_),
		targetY(targetY_),
		loops(loops_),
		active(true),
		startX(1),
		startY(1) {}

	void Start() override {
		TileTransform* transform = gameObject->GetTransform();
		startX = transform->GetScaleX();
		startY = transform->GetScaleY();
	}

	void Update(float deltaTime) override {
		if (!active) return;

		TileTransform* transform = gameObject->GetTransform();
		elapsedTime += deltaTime;

		float t = elapsedTime / duration;
		if (t >= 1.0f) {

			loops--;

			if (loops == 0)
			{
				active = false;  // Stop animation
				transform->SetScale(targetX, targetY);
				return;
			}
			
			// Swap start and target for ping-pong effect
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
};
