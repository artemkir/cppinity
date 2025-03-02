#pragma once

#include <random>

#include "MazeGenerator.h"
#include "Scene.h"

// SnakeLogic Component
class SnakeLogic : public BaseComponent {
private:
	MazeGenerator* mazeGenerator = nullptr;
	GameObject* apple = nullptr;
	RectRenderer* rect = nullptr;

	std::vector<GameObject*> tail;
	int score = 0;
	float updateTimer = 0.0f;
	float update_interval = 0.2f;
	float min_update_interval = 0.05f;
	float tail_change_interval = 0.005f;

	IInputHandler* inputHandler = nullptr;


	void RegenerateApple() {
		std::random_device rd;
		std::mt19937 g(rd());
		std::uniform_int_distribution<> disX(0, WIDTH - 1);
		std::uniform_int_distribution<> disY(0, HEIGHT - 1);

		while (true) {
			int newX = disX(g);
			int newY = disY(g);

			if (mazeGenerator->IsObstacle(newX, newY))
				continue;

			apple->GetTransform()->SetPosition(newX, newY);
			break;
		}

	}

public:
	void Start() override {
		inputHandler = gameObject->GetComponent<IInputHandler>();
		mazeGenerator = gameObject->GetScene()->FindGameObjectByName("maze_generator")->GetComponent<MazeGenerator>();
		apple = gameObject->GetScene()->FindGameObjectByName("apple");
		rect = gameObject->GetComponent<RectRenderer>();
	}

	void Update(float deltaTime) override {
		updateTimer += deltaTime;

		if (updateTimer < update_interval)
			return;

		updateTimer -= update_interval; // reset timer but keep overflow for accuracy

		auto dir = inputHandler->GetDirection();
		Transform* headTransform = gameObject->GetTransform();

		if (dir == Direction::STOP) return;

		int prevX = headTransform->GetX();
		int prevY = headTransform->GetY();

		switch (dir) {
		case Direction::LEFT: headTransform->SetPosition(prevX - 1, prevY); break;
		case Direction::RIGHT: headTransform->SetPosition(prevX + 1, prevY); break;
		case Direction::UP: headTransform->SetPosition(prevX, prevY - 1); break;
		case Direction::DOWN: headTransform->SetPosition(prevX, prevY + 1); break;
		default: break;
		}

		// Wrap around logic
		if (headTransform->GetX() < 0) headTransform->SetPosition(WIDTH - 1, headTransform->GetY());
		if (headTransform->GetX() >= WIDTH) headTransform->SetPosition(0, headTransform->GetY());
		if (headTransform->GetY() < 0) headTransform->SetPosition(headTransform->GetX(), HEIGHT - 1);
		if (headTransform->GetY() >= HEIGHT) headTransform->SetPosition(headTransform->GetX(), 0);

		for (size_t i = 0; i < tail.size(); i++) {
			Transform* tailTransform = tail[i]->GetTransform();
			int tempX = tailTransform->GetX();
			int tempY = tailTransform->GetY();
			tailTransform->SetPosition(prevX, prevY);
			prevX = tempX;
			prevY = tempY;

			tail[i]->GetComponent<SimpleCollider>()->SetActive(true);
		}

	}

	void OnCollide(GameObject* other) override {
		if (!other) return;

		if (other->GetName() == "apple") {
			score += 10;

			//move apple to new position
			RegenerateApple();


			Transform* headTransform = gameObject->GetTransform();
			int prevX = headTransform->GetX();
			int prevY = headTransform->GetY();
			if (!tail.empty()) {
				prevX = tail.back()->GetTransform()->GetX();
				prevY = tail.back()->GetTransform()->GetY();
			}
			auto newTail = std::make_unique<GameObject>("tail_" + std::to_string(tail.size()), OBSTACLE_TAG);

			auto color = rect->GetColor();

			newTail->AddComponent(std::make_unique<RectRenderer>((int)(color.r*0.75f), (int)(color.g * 0.75f), (int)(color.b * 0.75f)));
			newTail->AddComponent(std::make_unique<Transform>(prevX, prevY, TILE_SIZE, TILE_SIZE));
			newTail->AddComponent(std::make_unique<SimpleCollider>(false));
			tail.push_back(newTail.get());

			if (tail.size() > 0 && update_interval > min_update_interval)
			{
				update_interval -= tail_change_interval;
			}

			gameObject->GetScene()->AddGameObject(std::move(newTail));
		}
		else {
			//gameObject->GetScene()->Stop();
		}
	}

	int GetScore() { return score; }
};