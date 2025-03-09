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
#include <stdexcept>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

const int OBSTACLE_TAG = 1;

const int WIDTH = 40;
const int HEIGHT = 40;
const int TILE_SIZE = 10;

#include "DebugTextures.h"

#include "IRenderer.h"
#include "SDLRenderer.h"
#include "TexturesManager.h"

#include "Components/BaseComponent.h"
#include "Components/Transform.h"
#include "Components/RendererComponent.h"
#include "Components/SimpleCollider.h"
#include "Components/RectRenderer.h"
#include "Components/InputHandler.h"
#include "Components/Animation.h"

#include "Scripts/SnakeLogic.h"
#include "Scripts/MazeGenerator.h"
#include "Scripts/NPCInputHandler.h"
#include "Scripts/AppleLogic.h"

#include "Scene.h"
#include "GameObject.h"

#ifdef __EMSCRIPTEN__
struct LoopData { Scene* scene; };
void emscriptenLoop(void* arg) {
	LoopData* data = static_cast<LoopData*>(arg);
	if (!data->scene->MainLoop()) emscripten_cancel_main_loop();
}
#endif

#define ICON_HEIGHT 16 
#define ICON_WIDTH 16 
// array size is 256 
static const unsigned char icon[] = { 0x00, 0x00, 0x00, 0x00, 0xff, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfd, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xfd, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0xfc, 0xff, 0xfa, 0xc1, 0x90, 0x79, 0x79, 0x8f, 0xc0, 0xfa, 0xff, 0xfd, 0xfe, 0x00, 0x00, 0xfd, 0xff, 0xee, 0x8d, 0x76, 0x76, 0x76, 0x76, 0x76, 0x76, 0x9b, 0xff, 0xff, 0xfd, 0x00, 0xff, 0xfe, 0xfa, 0x8c, 0x76, 0x7a, 0xb7, 0xdf, 0xdf, 0xb7, 0x88, 0xea, 0xff, 0xff, 0xfd, 0xf9, 0xfd, 0xff, 0xca, 0x79, 0x79, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xfd, 0xff, 0xa8, 0x91, 0xb5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xfd, 0xff, 0x96, 0x94, 0xdf, 0xff, 0xff, 0xff, 0x93, 0x93, 0x93, 0x93, 0x93, 0xb3, 0xff, 0xfd, 0xfd, 0xff, 0x96, 0x94, 0xdf, 0xff, 0xff, 0xff, 0x93, 0x93, 0x93, 0x93, 0x93, 0xaf, 0xff, 0xfd, 0xfd, 0xff, 0xa9, 0x8f, 0xb0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb8, 0x93, 0xbd, 0xff, 0xfd, 0xfd, 0xff, 0xc8, 0x69, 0x68, 0xd9, 0xff, 0xff, 0xff, 0xff, 0xe9, 0x96, 0x93, 0xdc, 0xff, 0xff, 0xff, 0xfd, 0xfa, 0x7f, 0x65, 0x69, 0xac, 0xd9, 0xdb, 0xb3, 0x7e, 0x92, 0xa8, 0xfd, 0xfd, 0xf9, 0x00, 0xfd, 0xff, 0xed, 0x80, 0x65, 0x65, 0x65, 0x65, 0x65, 0x65, 0x82, 0xf1, 0xff, 0xfd, 0x00, 0x00, 0xfe, 0xfd, 0xff, 0xfa, 0xba, 0x84, 0x69, 0x69, 0x80, 0xb4, 0xf7, 0xff, 0xfd, 0xff, 0x00, 0x00, 0x00, 0xfe, 0xfd, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xfd, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xff, 0xf9, 0x00, 0x00, 0x00, 0x00 };

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
	TextureManager textureManager(renderer);

	auto scene = std::make_unique<Scene>(renderer);

	auto iconTexture = textureManager.LoadTexture("icon", ICON_WIDTH, ICON_HEIGHT, icon);

#ifdef DEBUG
	SDL_Texture* debugTexture = CreateDebugTexture2(sdlRenderer, icon, ICON_WIDTH, ICON_HEIGHT);
#endif	

	// Background
	/*auto background = std::make_unique<GameObject>("background");
	background->AddComponent(std::make_unique<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
	background->AddComponent(std::make_unique<RectRenderer>(0, 0, 0));
	scene->AddGameObject(std::move(background));*/

	// Border
	auto border = std::make_unique<GameObject>("border");
	border->AddComponent(std::make_unique<TileTransform>(0, 0, WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE));
	border->AddComponent(std::make_unique<RectRenderer>(255, 255, 255));
	scene->AddGameObject(std::move(border));

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