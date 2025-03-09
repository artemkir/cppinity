#pragma once

class AppleLogic : public BaseComponent
{
	MazeGenerator* mazeGenerator = nullptr;

public:
	void Start() override {
		mazeGenerator = gameObject->GetScene()->FindGameObjectByName("maze_generator")->GetComponent<MazeGenerator>();
		gameObject->GetTransform()->SetSize(TILE_SIZE, TILE_SIZE);
		RegenerateApple();
	}

	void RegenerateApple()
	{
		auto [x, y] = mazeGenerator->GetRandomEmptyPosition();
		gameObject->GetTransform()->SetPosition(x, y);
	}
};