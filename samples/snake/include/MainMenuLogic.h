#pragma once

#include "Engine.h"

//Header only script implementation

class MainMenuLogic : public BaseComponent
{
    GameObject *bg = nullptr;
    GameObject *startButton = nullptr;
    GameStateManager *stateManager = nullptr;

public:
    MainMenuLogic() = default;
        
    void Start()
    {
        auto scene = gameObject->GetScene();
        auto canvas = scene->FindGameObjectByName("MainCanvas");
		auto canvasSize = canvas->GetComponent<Canvas>()->GetCanvasSize();

        assert(canvas);

        // Menu Background
        bg = canvas->CreateGameObjectBuilder("MenuBackground", 0)
			//Centered in canvas, full size
            .WithComponent<ScreenTransform>(canvasSize*0.5f, canvasSize, Vector2{ 1.0f, 1.0f }, Vector2{ 0.5f, 0.5f })
            .WithComponent<SpriteRenderer>("start.png",2)
            .WithComponent<Animation>(0.5f, Vector2{ 1.1f, 1.1f }, -1)
            .AddToScene();

        auto stateManagerObject = scene->FindGameObjectByName("StateMachineRoot");

        assert(stateManagerObject && "State manager is null");
        stateManager = stateManagerObject->GetComponent<GameStateManager>();
    }

	void OnActive(bool isActive) override
	{
		if (bg)
		{
			bg->SetActive(isActive);
		}
	}   

    void Update(float deltaTime)
    {
        auto scene = gameObject->GetScene();
        auto& input = scene->GetInput();

        if (input.IsKeyPressed(Key::Space))
        {
            stateManager->TransitionTo(GameState::GameMode);
        }
    }
};
