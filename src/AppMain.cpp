#include <ctime>
#include <cstdlib>
#include <memory>

#include "InputEvent.h"

#include "IRenderer.h"
#include "ShaderManager.h"
#include "MaterialManager.h"
#include "RenderTypes.h"

#include "Components/Transform.h"
#include "Components/SimpleCollider.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Components/InputHandler.h"
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
#include "Input.h"

struct AppState
{
    std::unique_ptr<SokolRenderer> renderer;
    std::unique_ptr<TexturesManager> textureManager;
    std::unique_ptr<ShaderManager> shaderManager;
    std::unique_ptr<MaterialManager> materialManager;
    std::unique_ptr<Scene> scene;
};

static std::unique_ptr<AppState> app_state = nullptr;

void CreateInitialScene(Scene* scene)
{
	auto iconTexture = scene->GetTextureManager()->LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

	// Main Menu Root
	scene->CreateGameObjectBuilder("MainMenuRoot", 0)
		.WithComponent<MainMenuLogic>()
		.AddToScene();

	// Game Mode Root
	auto root = scene->CreateGameObjectBuilder("GameModeRoot", 0)
		.AddToScene();

	// Background (commented out, but refactored for completeness)
	/*scene->CreateGameObjectBuilder("background", 0)
		.WithComponent<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE)
		.WithComponent<RectRenderer>(0, 0, 0)
		.Build();*/

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
		//.WithComponent<SpriteRenderer>(iconTexture)
		.WithComponent<RectRenderer>(255, 0, 0, 100)  // Commented in original
		//.WithComponent<Animation>(0.25f, 1.25f, 1.25f, -1)  // Commented in original
		.WithComponent<Animation>(0.25f, 2.25f, 2.25f, -1)
		.AddToScene();

	// Snake Head
	scene->CreateGameObjectBuilder("snake_head", OBSTACLE_TAG)
		.WithComponent<TileTransform>()
		.WithComponent<RectRenderer>(0, 255, 0)
		.WithComponent<SimpleCollider>()
		.WithComponent<InputHandler>()
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
		.WithComponent<EndScreenLogic>()
		.AddToScene();

	// State Machine Root
	scene->CreateGameObjectBuilder("StateMachineRoot", 0)
		.WithComponent<GameStateManager>()
		.AddToScene();
}

void gameInit(Scene* scene)
{
	CreateInitialScene(scene);
}

extern "C" void app_init(void)
{
    app_state = std::make_unique<AppState>();

    app_state->renderer = std::make_unique<SokolRenderer>();
    app_state->textureManager = std::make_unique<TexturesManager>(app_state->renderer.get());
    app_state->shaderManager = std::make_unique<ShaderManager>();
    app_state->materialManager = std::make_unique<MaterialManager>(app_state->shaderManager.get());
    app_state->scene = std::make_unique<Scene>(
        app_state->renderer.get(),
        app_state->materialManager.get(),
        app_state->textureManager.get());
	 
    app_state->materialManager->CreateDefaultMaterials();
    
    gameInit(app_state->scene.get());
}

extern "C" bool app_frame(float deltaTime)
{
    return app_state->scene->Frame(deltaTime);
}

extern "C" void app_cleanup(void)
{
    app_state = nullptr;
}

extern "C" void app_event(const InputEvent *event)
{
    app_state->scene->HandleEvent(event);
}
