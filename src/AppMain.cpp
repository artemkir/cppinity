#include <ctime>
#include <cstdlib>
#include <memory>

#include "InputEvent.h"

#include "IRenderer.h"

#include "Components/Transform.h"
#include "Components/SimpleCollider.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Components/Animation.h"

#include "SokolRenderer.h"
#include "TexturesManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameObjectBuilder.h"

#include "Scripts/SnakeLogic.h"
#include "Scripts/MazeGenerator.h"
#include "Scripts/NPCInputHandler.h"
#include "Scripts/AppleLogic.h"
#include "Scripts/IconImage.h"
#include "Scripts/MainMenuLogic.h"
#include "Scripts/EndScreenLogic.h"
#include "Scripts/GameConsts.h"
#include "Scripts/GameStateManager.h"
#include "Input.h" // Updated input handler

struct AppState
{
    SokolRenderer renderer;
    TexturesManager textureManager;
    std::unique_ptr<Scene> scene;

    AppState()
        : textureManager(&renderer),
          scene(std::make_unique<Scene>(&renderer)) {}
};

static AppState *state = nullptr;

void gameInit(Scene *scene, TexturesManager &textureManager)
{
    auto iconTexture = state->textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    // Main Menu Root
    scene->CreateGameObjectBuilder("MainMenuRoot", 0)
        .WithComponent<MainMenuLogic>(textureManager)
        .AddToScene();

    // Game Mode Root
    auto root = scene->CreateGameObjectBuilder("GameModeRoot", 0)
                    .AddToScene();

    // Border (as child of root)
    root->CreateGameObjectBuilder("border", 0)
        .WithComponent<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE)
        .WithComponent<RectRenderer>(255, 255, 255)
        .AddToScene();

    // Maze Generator
    scene->CreateGameObjectBuilder("maze_generator", 0)
        .WithComponent<MazeGenerator>()
        .AddToScene();

    // Apple
    scene->CreateGameObjectBuilder("apple", OBSTACLE_TAG)
        .WithComponent<TileTransform>()
        .WithComponent<SimpleCollider>()
        .WithComponent<AppleLogic>()
        .WithComponent<SpriteRenderer>(iconTexture)
        .WithComponent<Animation>(0.25f, 2.25f, 2.25f, -1)
        .AddToScene();

    // Snake Head
    scene->CreateGameObjectBuilder("snake_head", OBSTACLE_TAG)
        .WithComponent<TileTransform>()
        .WithComponent<RectRenderer>(0, 255, 0)
        .WithComponent<SimpleCollider>()
        .WithComponent<SnakeLogic>()
        .AddToScene();

    // NPC Snake
    scene->CreateGameObjectBuilder("npc_snake_head", OBSTACLE_TAG)
        .WithComponent<TileTransform>()
        .WithComponent<RectRenderer>(170, 100, 200)
        .WithComponent<SnakeLogic>()
        .WithComponent<NPCInputHandler>()
        .WithComponent<SimpleCollider>()
        .AddToScene();

    // End Screen Root
    scene->CreateGameObjectBuilder("EndScreenRoot", 0)
        .WithComponent<EndScreenLogic>(textureManager)
        .AddToScene();

    // State Machine Root
    scene->CreateGameObjectBuilder("StateMachineRoot", 0)
        .WithComponent<GameStateManager>()
        .AddToScene();
}

extern "C" void app_init(void)
{

    state = new AppState();
    state->renderer = SokolRenderer();
    state->textureManager = TexturesManager(&state->renderer);
    state->scene = std::make_unique<Scene>(&state->renderer);

    gameInit(state->scene.get(), state->textureManager);
}

extern "C" bool app_frame(uint64_t deltaTime)
{
    return state->scene->Frame(deltaTime);
}

extern "C" void app_cleanup(void)
{
    delete state;
}

extern "C" void app_event(const InputEvent *event)
{
    state->scene->HandleEvent(event);
}
