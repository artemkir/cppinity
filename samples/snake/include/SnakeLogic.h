#pragma once

#include "Engine.h"

// SnakeLogic Component
class SnakeLogic : public BaseComponent
{
	MazeGenerator *mazeGenerator = nullptr;
	AppleLogic *apple = nullptr;
	RectRenderer *rect = nullptr;

	std::vector<GameObject *> tail;
	int score = 0;
	float updateTimer = 0.0f;
	float update_interval = 0.2f;
	float min_update_interval = 0.05f;
	float tail_change_interval = 0.005f;
	float blockSizeW = 0.0f;
	float blockSizeH = 0.0f;

	IInputHandler *inputHandler = nullptr;

public:
	void Awake() override;
	void Update(float deltaTime) override;
	void OnCollide(GameObject *other) override;
	int GetScore();
};