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
    UniquePtr<SokolRenderer> renderer;
    UniquePtr<ShaderManager> shaderManager;
    UniquePtr<MaterialManager> materialManager;
	UniquePtr<ResourceManager> resourceManager;
    UniquePtr<Scene> scene;
};

static UniquePtr<AppState> app_state = nullptr;

extern void gameInit(Scene *);

extern "C" void app_init(int screen_width, int screen_height)
{
    assert(!app_state && "AppState already initialized");
  
	assert(screen_width > 0 && screen_height > 0 && "Invalid screen dimensions");

    app_state = MakeUnique<AppState>();
		
    app_state->renderer = MakeUnique<SokolRenderer>(screen_width, screen_height);
	app_state->resourceManager = MakeUnique<ResourceManager>(app_state->renderer.get());
    app_state->shaderManager = MakeUnique<ShaderManager>();
    app_state->materialManager = MakeUnique<MaterialManager>(app_state->shaderManager.get());
    app_state->scene = MakeUnique<Scene>(
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
    assert (app_state && "AppState not initialized");
    
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