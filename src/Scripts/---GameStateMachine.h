#pragma once

#include <memory>
#include <string>
#include "IconImage.h"

class Scene;
class GameObject;
class StateMachine;
class SnakeLogic;
class NPCInputHandler;

#include "SnakeLogic.h"

class State {
protected:
    std::string name;
    Scene* scene;
    std::unique_ptr<GameObject> stateRoot;

public:
    explicit State(const std::string& name_, Scene* scene_) : name(name_), scene(scene_) {}
    virtual ~State() = default;

    virtual void Enter() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Exit() = 0;   

    const std::string& GetName() const { return name; }
    GameObject* GetRoot() const { return stateRoot.get(); }
};

class StateMachine : public BaseComponent {
private:
    std::unordered_map<std::string, std::unique_ptr<State>> states;
    State* currentState = nullptr;

public:
    void AddState(std::unique_ptr<State> state) {
        states[state->GetName()] = std::move(state);
    }

    void SetState(const std::string& stateName) {
        auto it = states.find(stateName);
        if (it == states.end()) {
            throw std::runtime_error("State not found: " + stateName);
        }

        if (currentState) {
            currentState->Exit();
        }
        currentState = it->second.get();
        currentState->Enter();
    }

    void Start() override {
        if (!states.empty()) {
            SetState(states.begin()->second->GetName()); // Default to first state
        }
    }

    void Update(float deltaTime) override {
        if (currentState) {
            currentState->Update(deltaTime);
        }
    }

    State* GetCurrentState() const { return currentState; }
};

class MainMenuLogic : public BaseComponent {
private:
    TextureManager& textureManager;
    std::shared_ptr<ITexture> buttonTexture;

public:
    MainMenuLogic(TextureManager& textureManager_)
        : textureManager(textureManager_) {}

    void Start() override {
        // Load a simple texture for the "Start" button (using icon as placeholder)
        buttonTexture = textureManager.LoadTexture("icon", 128, 128, icon);

        // Add background
        auto bg = std::make_unique<GameObject>("MenuBackground");
        bg->AddComponent(std::make_unique<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
        bg->AddComponent(std::make_unique<RectRenderer>(50, 50, 50));
        gameObject->GetScene()->AddGameObject(std::move(bg));

        // Add Start button
        auto startButton = std::make_unique<GameObject>("StartButton");
        startButton->AddComponent(std::make_unique<TileTransform>(WIDTH / 2 - 2, HEIGHT / 2 - 2, 32, 32));
        startButton->AddComponent(std::make_unique<SpriteRenderer>(buttonTexture));
        gameObject->GetScene()->AddGameObject(std::move(startButton));
    }

    void Update(float deltaTime) override {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameObject->GetScene()->Stop();
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                // Transition to GameMode
                auto stateMachine = gameObject->GetComponent<StateMachine>();
                stateMachine->SetState("GameMode");
            }
        }
    }
};

class MainMenuState : public State {
private:
    TextureManager& textureManager;

public:
    MainMenuState(Scene* scene_, TextureManager& textureManager_)
        : State("MainMenu", scene_), textureManager(textureManager_) {}

    void Enter() override {
        stateRoot = std::make_unique<GameObject>("MainMenuRoot");
        stateRoot->AddComponent(std::make_unique<MainMenuLogic>(textureManager));
        scene->AddGameObject(std::move(stateRoot));
    }

    void Update(float deltaTime) override {
        // Handled by components in the state's GameObject hierarchy
    }

    void Exit() override {
        stateRoot.reset(); // Cleanup happens via Scene's ownership
    }
};

class GameModeState : public State {
private:
    TextureManager& textureManager;
    std::shared_ptr<ITexture> iconTexture;

public:
    GameModeState(Scene* scene_, TextureManager& textureManager_)
        : State("GameMode", scene_), textureManager(textureManager_) {}

    void Enter() override {
        stateRoot = std::make_unique<GameObject>("GameModeRoot");
        iconTexture = textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

        // Maze Generator
        /*auto maze = std::make_unique<GameObject>("maze_generator");
        maze->AddComponent(std::make_unique<MazeGenerator>());
        scene->AddGameObject(std::move(maze));

        // Apple
        auto apple = std::make_unique<GameObject>("apple", OBSTACLE_TAG);
        apple->AddComponent(std::make_unique<TileTransform>());
        apple->AddComponent(std::make_unique<SimpleCollider>());
        apple->AddComponent(std::make_unique<AppleLogic>());
        apple->AddComponent(std::make_unique<SpriteRenderer>(iconTexture));
        apple->AddComponent(std::make_unique<Animation>(0.25f, 2.25f, 2.25f, -1));
        scene->AddGameObject(std::move(apple));

        // Snake Head
        auto snake = std::make_unique<GameObject>("snake_head", OBSTACLE_TAG);
        snake->AddComponent(std::make_unique<TileTransform>());
        snake->AddComponent(std::make_unique<RectRenderer>(0, 255, 0));
        snake->AddComponent(std::make_unique<SimpleCollider>());
        snake->AddComponent(std::make_unique<InputHandler>());
        snake->AddComponent(std::make_unique<SnakeLogic>());
        scene->AddGameObject(std::move(snake));

        // NPC Snake
        auto npcSnake = std::make_unique<GameObject>("npc_snake_head", OBSTACLE_TAG);
        npcSnake->AddComponent(std::make_unique<TileTransform>());
        npcSnake->AddComponent(std::make_unique<RectRenderer>(170, 100, 200));
        npcSnake->AddComponent(std::make_unique<SnakeLogic>());
        npcSnake->AddComponent(std::make_unique<NPCInputHandler>());
        npcSnake->AddComponent(std::make_unique<SimpleCollider>());
        scene->AddGameObject(std::move(npcSnake));*/

        // Add StateMachine to root for transitions
        stateRoot->AddComponent(std::make_unique<StateMachine>());
        scene->AddGameObject(std::move(stateRoot));
    }

    void Update(float deltaTime) override {
        // Handled by game objects in the scene
    }

    void Exit() override {
        stateRoot.reset(); // Cleanup via Scene
    }
};

class EndScreenLogic : public BaseComponent {
private:
    TextureManager& textureManager;
    int score;
    std::shared_ptr<ITexture> texture;

public:
    EndScreenLogic(TextureManager& textureManager_, int score_)
        : textureManager(textureManager_), score(score_) {}

    void Start() override {
        texture = textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

        // Add background
        auto bg = std::make_unique<GameObject>("EndBackground");
        bg->AddComponent(std::make_unique<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
        bg->AddComponent(std::make_unique<RectRenderer>(20, 20, 20));
        gameObject->GetScene()->AddGameObject(std::move(bg));

        // Add "Game Over" indicator (using icon as placeholder)
        auto gameOver = std::make_unique<GameObject>("GameOverText");
        gameOver->AddComponent(std::make_unique<TileTransform>(WIDTH / 2 - 2, HEIGHT / 2 - 4, 32, 32));
        gameOver->AddComponent(std::make_unique<SpriteRenderer>(texture));
        gameObject->GetScene()->AddGameObject(std::move(gameOver));
    }

    void Update(float deltaTime) override {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)) {
                auto stateMachine = gameObject->GetComponent<StateMachine>();
                stateMachine->SetState("MainMenu"); // Restart by going back to Main Menu
            }
        }
    }
};

class EndScreenState : public State {
private:
    TextureManager& textureManager;
    int finalScore = 0;

public:
    EndScreenState(Scene* scene_, TextureManager& textureManager_, int score_)
        : State("EndScreen", scene_), textureManager(textureManager_), finalScore(score_) {}

    void Enter() override {
        stateRoot = std::make_unique<GameObject>("EndScreenRoot");
        stateRoot->AddComponent(std::make_unique<EndScreenLogic>(textureManager, finalScore));
        scene->AddGameObject(std::move(stateRoot));
    }

    void Update(float deltaTime) override {
        // Handled by components
    }

    void Exit() override {
        stateRoot.reset();
    }
};