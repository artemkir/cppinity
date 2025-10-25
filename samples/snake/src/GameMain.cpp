#include "Engine.h"

#include "Game.h"

void CreateInitialScene(Scene* scene)
{
	//Load Resources
	scene->GetResourceManager()->Load<Texture>("win.png");
	scene->GetResourceManager()->Load<Texture>("lose.png");
	scene->GetResourceManager()->Load<Texture>("start.png");

	scene->CreateGameObjectBuilder("MainCanvas")
		.WithComponent<Canvas>(Vector2{ -1.0f, 1.0f }, Vector2{ 0.5f, 0.5f }, AnchorType::Center, Vector2{ 1000.0f, 1000.0f })
		.AddToScene();

	// Main Menu Root 
	scene->CreateGameObjectBuilder("MainMenuRoot", 0)
		.WithComponent<MainMenuLogic>()
		.AddToScene();

	return;
	// Maze Generator
	scene->CreateGameObjectBuilder("maze_generator", 0)
		.WithComponent<MazeGenerator>()
		.AddToScene();

	// Apple
	scene->CreateGameObjectBuilder("apple", OBSTACLE_TAG)
		.WithComponent<GridTransform>(Vector2{ 0,0 }, Vector2{ 1, 1 })
		.WithComponent<SimpleCollider>()
		.WithComponent<AppleLogic>()
		.WithComponent<RectRenderer>(255, 0, 0, 100)  
		.WithComponent<Animation>(0.25f, 1.25f, 1.25f, -1)
		.AddToScene();

	// Snake Head
	scene->CreateGameObjectBuilder("snake_head", OBSTACLE_TAG)
		.WithComponent<GridTransform>(Vector2{ 0, 0 }, Vector2{ 1, 1 })
		.WithComponent<RectRenderer>(0, 255, 0)
		.WithComponent<SimpleCollider>()
		.WithComponent<InputHandler>()
		.WithComponent<SnakeLogic>()
		.AddToScene();

	// NPC Snake
	scene->CreateGameObjectBuilder("npc_snake_head", OBSTACLE_TAG)
		.WithComponent<GridTransform>(Vector2{ 0, 0 }, Vector2{ 1, 1 })
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