//#define __EMSCRIPTEN__

#include <SDL2/SDL.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <string>
#include <stack>
#include <queue>
#include <set>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

const int OBSTACLE_TAG = 1;

const int WIDTH = 40;
const int HEIGHT = 40;
const int TILE_SIZE = 10;

#include "IRenderer.h"
#include "SDLRenderer.h"

#include "Components/BaseComponent.h"
#include "Components/Transform.h"
#include "Components/IRendererComponent.h"
#include "Components/SimpleCollider.h"
#include "Components/RectRenderer.h"
#include "Components/InputHandler.h"

#include "Scripts/SnakeLogic.h"
#include "Scripts/MazeGenerator.h"
#include "Scripts/NPCInputHandler.h"

#include "Scene.h"
#include "GameObject.h"


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
	auto scene = std::make_unique<Scene>(renderer);

	// Background
	auto background = std::make_unique<GameObject>("background");
	background->AddComponent(std::make_unique<Transform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
	background->AddComponent(std::make_unique<RectRenderer>(0, 0, 0));
	scene->AddGameObject(std::move(background));

	// Border
	auto border = std::make_unique<GameObject>("border");
	border->AddComponent(std::make_unique<Transform>(0, 0, WIDTH * TILE_SIZE + 2, HEIGHT * TILE_SIZE + 2));
	border->AddComponent(std::make_unique<RectRenderer>(255, 255, 255));
	scene->AddGameObject(std::move(border));

	// Apple
	auto apple = std::make_unique<GameObject>("apple", OBSTACLE_TAG);
	apple->AddComponent(std::make_unique<Transform>(rand() % WIDTH, rand() % HEIGHT, TILE_SIZE, TILE_SIZE));
	apple->AddComponent(std::make_unique<SimpleCollider>());
	apple->AddComponent(std::make_unique<RectRenderer>(255, 0, 0, 100));
	scene->AddGameObject(std::move(apple));

	// Maze Generator
	auto maze = std::make_unique<GameObject>("maze_generator");
	maze->AddComponent(std::make_unique<MazeGenerator>());
	scene->AddGameObject(std::move(maze));

	// Snake Head
	auto snake = std::make_unique<GameObject>("snake_head", OBSTACLE_TAG);
	snake->AddComponent(std::make_unique<Transform>(WIDTH / 2, HEIGHT / 2, TILE_SIZE, TILE_SIZE));
	snake->AddComponent(std::make_unique<RectRenderer>(0, 255, 0));
	snake->AddComponent(std::make_unique<SimpleCollider>());
	snake->AddComponent(std::make_unique<InputHandler>());
	snake->AddComponent(std::make_unique<SnakeLogic>());
	scene->AddGameObject(std::move(snake));

    // NPC Snake
    auto npcSnake = std::make_unique<GameObject>("npc_snake_head", OBSTACLE_TAG); 
    npcSnake->AddComponent(std::make_unique<Transform>(1, 1, TILE_SIZE, TILE_SIZE));
    npcSnake->AddComponent(std::make_unique<RectRenderer>(170, 100, 200));  
    npcSnake->AddComponent(std::make_unique<SnakeLogic>());
    npcSnake->AddComponent(std::make_unique<SimpleCollider>());
    npcSnake->AddComponent(std::make_unique<NPCInputHandler>());
    npcSnake->AddComponent(std::make_unique<SimpleCollider>());
    scene->AddGameObject(std::move(npcSnake));

	//scene.Start();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(emscriptenLoop, &scene, 10, 1);
#else
	bool running = true;
	while (scene->MainLoop()) {
		SDL_Delay(16); // ~60fps rendering delay
	}
	//SDL_Log("Game Over! Final Score: %d", snakeLogicPtr->GetScore());
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
#endif

	return 0;
}