#pragma once

// Animation Controller Component
class Animation : public BaseComponent {
private:
	float duration;				// Animation duration in seconds
	float elapsedTime = 0.0f;	// Time elapsed since animation started
	float startX = 0.0f;		// Starting position
	float startY = 0.0f;   
	float targetX;				// Target position
	float targetY; 
	int loops;					// Number of loops, -1 infinite loop
	bool active = true;			// Whether the animation is running

	float EaseInOutQuad(float t) const {
		t = std::max(0.0f, std::min(1.0f, t));  // Clamp t between 0 and 1
		return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
	}

public:
	Animation(float duration_, float targetX_, float targetY_, int loops_ = 1)
		:
		duration(duration_),
		targetX(targetX_),
		targetY(targetY_),
		loops(loops_)
	{
		if (duration_ <= 0.0f) {
			duration = 0.001f;  // Prevent division by zero
		}
	}

	void Start() override {
		const TileTransform* transform = gameObject->GetTransform();
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
