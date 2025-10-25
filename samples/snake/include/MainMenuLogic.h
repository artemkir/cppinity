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

        //float screenW = (float)gameObject->GetScene()->GetRenderer()->GetW();
        //float screenH = (float)gameObject->GetScene()->GetRenderer()->GetH();

        auto canvas = scene->FindGameObjectByName("MainCanvas");

		auto canvasSize = canvas->GetComponent<Canvas>()->GetCanvasSize();

        assert(canvas);

        // Menu Background
        bg = canvas->CreateGameObjectBuilder("MenuBackground", 0)
            .WithComponent<ScreenTransform>(Vector2{ 0.5f, 0.5f }, canvasSize, Vector2{ 1.0f, 1.0f }, Vector2{ 0.0f, 0.0f })
            ///.WithComponent<SpriteRenderer>("start.png",2)
            .WithComponent<SpriteRenderer>("default_texture",2)
            //.WithComponent<Animation>(0.5f, 1.1f, 1.1f, -1)
            .AddToScene();

        return;

        auto stateManagerObject = scene->FindGameObjectByName("StateMachineRoot");

        assert(stateManager && "State manager is null");
        stateManager = stateManagerObject->GetComponent<GameStateManager>();
    }

    void Update(float deltaTime)
    {
        return;

        auto scene = gameObject->GetScene();
        auto& input = scene->GetInput();

        if (input.IsKeyPressed(Key::Space))
        {
            stateManager->TransitionTo(GameState::GameMode);
        }
    }
};
