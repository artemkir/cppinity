#include "Engine.h"

#include "Game.h"

void CreateInitialScene(Scene* scene)
{
	//Load Resources
	scene->GetResourceManager()->Load<Texture>("win.png");
	scene->GetResourceManager()->Load<Texture>("lose.png");
	scene->GetResourceManager()->Load<Texture>("start.png");

	// Main Menu Root 
	scene->CreateGameObjectBuilder("MainMenuRoot", 0)
		.WithComponent<MainMenuLogic>()
		.AddToScene();

	// Maze Generator
	scene->CreateGameObjectBuilder("maze_generator", 0)
		.WithComponent<MazeGenerator>()
		.AddToScene();

	// Apple
	scene->CreateGameObjectBuilder("apple", OBSTACLE_TAG)
		.WithComponent<GridTransform>(0,0,1,1)
		.WithComponent<SimpleCollider>()
		.WithComponent<AppleLogic>()
		.WithComponent<RectRenderer>(255, 0, 0, 100)  
		.WithComponent<Animation>(0.25f, 1.25f, 1.25f, -1)
		.AddToScene();

	// Snake Head
	scene->CreateGameObjectBuilder("snake_head", OBSTACLE_TAG)
		.WithComponent<GridTransform>(0, 0, 1, 1)
		.WithComponent<RectRenderer>(0, 255, 0)
		.WithComponent<SimpleCollider>()
		.WithComponent<InputHandler>()
		.WithComponent<SnakeLogic>()
		.AddToScene();

	// NPC Snake
	scene->CreateGameObjectBuilder("npc_snake_head", OBSTACLE_TAG)
		.WithComponent<GridTransform>(0, 0, 1, 1)
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