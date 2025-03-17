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
	auto mainMenuRoot = std::make_unique<GameObject>("MainMenuRoot");
	mainMenuRoot->AddComponent(std::make_unique<MainMenuLogic>(textureManager));
	scene->AddGameObject(std::move(mainMenuRoot));

	auto gameModeRoot = std::make_unique<GameObject>("GameModeRoot");
	GameObject* root = gameModeRoot.get();

	scene->AddGameObject(std::move(gameModeRoot));

	
	// Background
	/*auto background = std::make_unique<GameObject>("background");
	background->AddComponent(std::make_unique<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
	background->AddComponent(std::make_unique<RectRenderer>(0, 0, 0));
	scene->AddGameObject(std::move(background));*/

	// Border
	auto border = std::make_unique<GameObject>("border");
	border->AddComponent(std::make_unique<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
	border->AddComponent(std::make_unique<RectRenderer>(255, 255, 255));
	//scene->AddGameObject(std::move(border));

	root->AddGameObject(std::move(border));

	// Maze Generator
	auto maze = std::make_unique<GameObject>("maze_generator");
	maze->AddComponent(std::make_unique<MazeGenerator>());
	scene->AddGameObject(std::move(maze));

	// Apple
	auto apple = std::make_unique<GameObject>("apple", OBSTACLE_TAG);
	apple->AddComponent(std::make_unique<TileTransform>());
	apple->AddComponent(std::make_unique<SimpleCollider>());
	apple->AddComponent(std::make_unique<AppleLogic>());
	apple->AddComponent(std::make_unique<SpriteRenderer>(iconTexture));
	//apple->AddComponent(std::make_unique<RectRenderer>(255, 0, 0, 100));
	//apple->AddComponent(std::make_unique<Animation>(0.25f, 1.25f, 1.25f, -1));

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
	scene->AddGameObject(std::move(npcSnake));
		
	// End Screen Root
	auto endScreenRoot = std::make_unique<GameObject>("EndScreenRoot");
	endScreenRoot->AddComponent(std::make_unique<EndScreenLogic>(textureManager));
	scene->AddGameObject(std::move(endScreenRoot));

	// State Machine Root
	auto stateMachineRoot = std::make_unique<GameObject>("StateMachineRoot");
	stateMachineRoot->AddComponent(std::make_unique<GameStateManager>());
	scene->AddGameObject(std::move(stateMachineRoot));

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