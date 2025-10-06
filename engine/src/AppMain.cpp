#include <ctime>
#include <cstdlib>
#include <memory>

#include "InputEvent.h"

#include "IRenderer.h"
#include "ShaderManager.h"
#include "MaterialManager.h"
#include "ResourceManager.h"
#include "RenderTypes.h"

#include "Components/Transform.h"
#include "Components/SimpleCollider.h"
#include "Components/RectRenderer.h"
#include "Components/SpriteRenderer.h"
#include "Components/InputHandler.h"
#include "Components/Animation.h"

#include "SokolRenderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameObjectBuilder.h"

#include "Input.h"

struct AppState
{
    std::unique_ptr<SokolRenderer> renderer;
    std::unique_ptr<ShaderManager> shaderManager;
    std::unique_ptr<MaterialManager> materialManager;
	std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<Scene> scene;
};

static std::unique_ptr<AppState> app_state = nullptr;

extern void gameInit(Scene *);

extern "C" void app_init(int screen_width, int screen_height)
{
    if (app_state) {
		throw std::runtime_error("AppState already initialized");
    }

    if (screen_width <= 0 || screen_height <= 0) {
		throw std::runtime_error("Invalid screen dimensions");
    }

    app_state = std::make_unique<AppState>();
		
    app_state->renderer = std::make_unique<SokolRenderer>(screen_width, screen_height);
	app_state->resourceManager = std::make_unique<ResourceManager>(app_state->renderer.get());
    app_state->shaderManager = std::make_unique<ShaderManager>();
    app_state->materialManager = std::make_unique<MaterialManager>(app_state->shaderManager.get());
    app_state->scene = std::make_unique<Scene>(
        app_state->renderer.get(),
        app_state->materialManager.get(),
        app_state->resourceManager.get());
	
	app_state->shaderManager->CreateDefaultShaders();
    app_state->materialManager->CreateDefaultMaterials();
    
    //-----

    const uint8_t pink[] = { 255,0,255,255 };
    app_state->resourceManager->CreateEmpty<Texture>("default_texture")->CreateRGBATextureFromPixelData(1, 1, pink);

    gameInit(app_state->scene.get());
}

extern "C" bool app_frame(float deltaTime)
{
    if (!app_state) {
		throw std::runtime_error("AppState not initialized");
    }

	app_state->resourceManager->Update();

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

extern "C" void app_fetch_completed(const uint8_t* data, size_t size, void* user)
{
	app_state->resourceManager->OnFetchComplete(data, size, false, nullptr, user);
}

extern "C" void app_fetch_failed(void* user)
{
	app_state->resourceManager->OnFetchComplete(nullptr, 0, true, "Failed to fetch resource", user);
}