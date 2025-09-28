//#define __EMSCRIPTEN__

#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "IRenderer.h"

#include "Components/Transform.h"
#include "Components/SimpleCollider.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Components/InputHandler.h"
#include "Components/Animation.h"


#include "SDLRenderer.h"
#include "TexturesManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameObjectBuilder.h"  // Include the builder header


#include "Scripts/SnakeLogic.h"
#include "Scripts/MazeGenerator.h"
#include "Scripts/NPCInputHandler.h"
#include "Scripts/AppleLogic.h"
#include "Scripts/IconImage.h"
#include "Scripts/MainMenuLogic.h"
#include "Scripts/EndScreenLogic.h"
#include "Scripts/GameConsts.h"
#include "Scripts/GameStateManager.h"


#ifdef __EMSCRIPTEN__
struct LoopData { Scene* scene; };
void emscriptenLoop(void* arg) {
	LoopData* data = static_cast<LoopData*>(arg);
	if (!data->scene->MainLoop()) emscripten_cancel_main_loop();
}
#endif

int main(int argc, char* argv[]) {
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Snake Game",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH * TILE_SIZE + 2, HEIGHT * TILE_SIZE + 2,
		SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_Log("Window creation failed: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!sdlRenderer) {
		SDL_Log("Renderer creation failed: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDLRenderer renderer(sdlRenderer);
	TexturesManager textureManager(&renderer);

	auto scene = std::make_unique<Scene>(&renderer);

	auto iconTexture = textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

#ifdef DEBUG
	SDL_Texture* debugTexture = CreateDebugTexture2(sdlRenderer, icon, ICON_WIDTH, ICON_HEIGHT);
#endif	

	// Main Menu Root
	scene->CreateGameObjectBuilder("MainMenuRoot", 0)
		.WithComponent<MainMenuLogic>(textureManager)
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
		.WithComponent<SpriteRenderer>(iconTexture)
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
		.WithComponent<EndScreenLogic>(textureManager)
		.AddToScene();

	// State Machine Root
	scene->CreateGameObjectBuilder("StateMachineRoot", 0)
		.WithComponent<GameStateManager>()
		.AddToScene();

	//scene.Start();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(emscriptenLoop, &scene, 10, 1);
#else
	bool running = true;
	while (scene->MainLoop()) {
		SDL_Delay(16); // ~60fps rendering delay

		// DEBUG: Render the debug texture at top-left corner
		//#ifdef DEBUG
		//RenderDebugTexture(sdlRenderer, debugTexture, 5, 5, 64, 64);
		//renderer.Present();
		//#endif

	}
	//SDL_Log("Game Over! Final Score: %d", snakeLogicPtr->GetScore());
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
#endif

	return 0;
}