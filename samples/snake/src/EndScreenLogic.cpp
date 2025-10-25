#include "Engine.h"

#include "Game.h"

void EndScreenLogic::Start()
{
	auto scene = gameObject->GetScene();
	auto canvas = scene->FindGameObjectByName("MainCanvas");
	auto canvasSize = canvas->GetComponent<Canvas>()->GetCanvasSize();

	// End Background
	bg = canvas->CreateGameObjectBuilder("EndBackground", 0)
		//Centered in canvas, full size
		.WithComponent<ScreenTransform>(canvasSize * 0.5f, canvasSize, Vector2{ 1.0f, 1.0f }, Vector2{ 0.5f, 0.5f })
		.WithComponent<SpriteRenderer>("lose.png", 2)
		.WithComponent<Animation>(0.5f, Vector2{ 1.1f, 1.1f }, -1)
		.AddToScene();

	stateManager = scene->FindGameObjectByName("StateMachineRoot")
		->GetComponent<GameStateManager>();
}

void EndScreenLogic::OnActive(bool isActive)
{
	if (bg)
	{
		bg->SetActive(isActive);
	}
}

void EndScreenLogic::Update(float deltaTime)
{
	auto scene = gameObject->GetScene();
	auto& input = scene->GetInput();

	if (input.IsKeyPressed(Key::Space))
	{
		if (stateManager)
		{
			stateManager->TransitionTo(GameState::MainMenu);
		}
	}
}