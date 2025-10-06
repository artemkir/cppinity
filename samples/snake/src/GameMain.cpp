#include "Engine.h"

#include "Game.h"

void CreateInitialScene(Scene* scene)
{
	//scene->GetTextureManager()->CreateTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

	std::shared_ptr<Texture> defaultTexture =
		scene->GetResourceManager()->CreateEmpty<Texture>("default_texture");

	const uint8_t pink[] = { 255,0,255,255 };

	defaultTexture->CreateRGBATextureFromPixelData(1, 1,  pink);


	std::shared_ptr<Texture> existingTexture = 
		scene->GetResourceManager()->CreateEmpty<Texture>("icon");

	existingTexture->CreateTextureFromGrayscalePixelData(ICON_WIDTH, ICON_HEIGHT, icon);

	
	scene->GetResourceManager()->Load<Texture>("broommaster.png");
	scene->GetResourceManager()->Load<Texture>("girl.png");

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
		.WithComponent<SpriteRenderer>("icon")
		//.WithComponent<RectRenderer>(255, 0, 0, 100)  // Commented in original
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