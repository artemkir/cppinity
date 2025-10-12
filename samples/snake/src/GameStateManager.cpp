#include "Engine.h"

#include "Game.h"

void GameStateManager::Start()
{
    mainMenuRoot = gameObject->GetScene()->FindGameObjectByName("MainMenuRoot");
    endScreenRoot = gameObject->GetScene()->FindGameObjectByName("EndScreenRoot");

    snake = gameObject->GetScene()->FindGameObjectByName("snake_head");
    player = gameObject->GetScene()->FindGameObjectByName("npc_snake_head");

    TransitionTo(GameState::MainMenu);
}

void GameStateManager::TransitionTo(GameState newState)
{
    // Deactivate all roots
    if (mainMenuRoot)
        mainMenuRoot->SetActive(false);
    if (endScreenRoot)
        endScreenRoot->SetActive(false);

    // Activate the new state
    currentState = newState;
    switch (currentState)
    {
    case GameState::MainMenu:
        mainMenuRoot->SetActive(true);
        break;
    case GameState::GameMode:
		snake->SetActive(true);
        player->SetActive(true);
        break;
    case GameState::EndScreen:
        endScreenRoot->SetActive(true);
        snake->SetActive(false);
        player->SetActive(false);
        break;
    }
}

GameState GameStateManager::GetCurrentState() const { return currentState; }
