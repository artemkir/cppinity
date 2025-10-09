#pragma once

#include "Engine.h"

enum class GameState
{
    MainMenu,
    GameMode,
    EndScreen
};

class GameStateManager : public BaseComponent
{
private:
    GameState currentState = GameState::MainMenu;
    GameObject *mainMenuRoot = nullptr;
    GameObject *gameModeRoot = nullptr;
    GameObject *endScreenRoot = nullptr;
    GameObject *snake = nullptr;
    GameObject *player = nullptr;

public:
    void Start() override;
    void TransitionTo(GameState newState);
    GameState GetCurrentState() const;
};