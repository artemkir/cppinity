// game.cpp (updated for sokol_app)


#ifdef __cplusplus
extern "C" {
#endif

#define SOKOL_GLCORE
#define SOKOL_IMPL
#define SOKOL_NO_DEPRECATED
#define SOKOL_CPP_NO_SHORTCUTS

#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#include "sokol_log.h"

#ifdef __cplusplus
}
#endif

#include <ctime>
#include <cstdlib>
#include <memory>

#include "IRenderer.h"

#include "Components/Transform.h"
#include "Components/SimpleCollider.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Components/Animation.h"


#include "Renderer.h"
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
#include "Input.h"  // Updated input handler

static void slog_func(const char* tag, uint32_t log_level, uint32_t log_item_id, const char* message, uint32_t line_nr, const char* filename, void* user_data) {
    if (message) {
        printf("[%s] %s\n", tag, message);
    }
}

struct AppState {
    Renderer renderer;
    TexturesManager textureManager;
    std::unique_ptr<Scene> scene;

    AppState()
        : textureManager(&renderer),  
        scene(std::make_unique<Scene>(&renderer)) {} 
};

static AppState* state = nullptr;
static uint64_t last_time = 0;

static void init(void) {
    
    sg_desc desc = {};
    desc.logger.func = slog_func;

    sg_setup(&desc);
         
    stm_setup();
    last_time = stm_now();

    state = new AppState();
    state->textureManager = TexturesManager(&state->renderer);
    state->scene = std::make_unique<Scene>(&state->renderer);

    srand(static_cast<unsigned>(stm_sec(stm_now())));

    auto iconTexture = state->textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

    // Main Menu Root
    state->scene->CreateGameObjectBuilder("MainMenuRoot", 0)
        .WithComponent<MainMenuLogic>(state->textureManager)
        .AddToScene();

    // Game Mode Root
    auto root = state->scene->CreateGameObjectBuilder("GameModeRoot", 0)
        .AddToScene();

    // Border (as child of root)
    root->CreateGameObjectBuilder("border", 0)
        .WithComponent<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE)
        .WithComponent<RectRenderer>(255, 255, 255)
        .AddToScene();

    // Maze Generator
    state->scene->CreateGameObjectBuilder("maze_generator", 0)
        .WithComponent<MazeGenerator>()
        .AddToScene();

    // Apple
    state->scene->CreateGameObjectBuilder("apple", OBSTACLE_TAG)
        .WithComponent<TileTransform>()
        .WithComponent<SimpleCollider>()
        .WithComponent<AppleLogic>()
        .WithComponent<SpriteRenderer>(iconTexture)
        .WithComponent<Animation>(0.25f, 2.25f, 2.25f, -1)
        .AddToScene();

    // Snake Head
    state->scene->CreateGameObjectBuilder("snake_head", OBSTACLE_TAG)
        .WithComponent<TileTransform>()
        .WithComponent<RectRenderer>(0, 255, 0)
        .WithComponent<SimpleCollider>()
        .WithComponent<SnakeLogic>()
        .AddToScene();

    // NPC Snake
    state->scene->CreateGameObjectBuilder("npc_snake_head", OBSTACLE_TAG)
        .WithComponent<TileTransform>()
        .WithComponent<RectRenderer>(170, 100, 200)
        .WithComponent<SnakeLogic>()
        .WithComponent<NPCInputHandler>()
        .WithComponent<SimpleCollider>()
        .AddToScene();

    // End Screen Root
    state->scene->CreateGameObjectBuilder("EndScreenRoot", 0)
        .WithComponent<EndScreenLogic>(state->textureManager)
        .AddToScene();

    // State Machine Root
    state->scene->CreateGameObjectBuilder("StateMachineRoot", 0)
        .WithComponent<GameStateManager>()
        .AddToScene();
}

static void frame(void) {
    uint64_t now = stm_now();
    float deltaTime = stm_sec(stm_diff(now, last_time));
    last_time = now;

    state->scene->Frame(deltaTime);
}

static void cleanup(void) {
    sg_shutdown();
    delete state;
}

static void event(const sapp_event* e) {
    state->scene->HandleEvent(e);
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (sapp_desc) {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = WIDTH * TILE_SIZE + 2,
        .height = HEIGHT * TILE_SIZE + 2,
        .sample_count = 4,
        .window_title = "Snake Game",
        .logger.func = slog_func,
    };
}