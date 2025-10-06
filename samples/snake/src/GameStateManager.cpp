#include "Engine.h"

#include "Game.h"

void GameStateManager::Start()
{
    mainMenuRoot = gameObject->GetScene()->FindGameObjectByName("MainMenuRoot");
    gameModeRoot = gameObject->GetScene()->FindGameObjectByName("GameModeRoot");
    endScreenRoot = gameObject->GetScene()->FindGameObjectByName("EndScreenRoot");
    TransitionTo(GameState::MainMenu);
}

void GameStateManager::TransitionTo(GameState newState)
{

    // Deactivate all roots
    if (mainMenuRoot)
        mainMenuRoot->SetActive(false);
    if (gameModeRoot)
        gameModeRoot->SetActive(false);
    if (endScreenRoot)
        endScreenRoot->SetActive(false);

    // Activate the new state
    currentState = newState;
    switch (currentState)
    {
    case GameState::MainMenu:
        if (mainMenuRoot)
            mainMenuRoot->SetActive(true);
        break;
    case GameState::GameMode:
        if (gameModeRoot)
            gameModeRoot->SetActive(true);
        break;
    case GameState::EndScreen:
        if (endScreenRoot)
        {
            endScreenRoot->SetActive(true);
            // Uncomment and use if score logic is required
            // auto logic = endScreenRoot->GetComponent<EndScreenLogic>();
            // if (logic) logic->SetScore(finalScore);
        }
        break;
    }
}

GameState GameStateManager::GetCurrentState() const { return currentState; }
