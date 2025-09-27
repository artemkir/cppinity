#include "Scene.h"
#include "GameObject.h"
#include "Components/RendererComponent.h"
#include "Components/SimpleCollider.h"
#include "IRenderer.h"
#include "GameObjectBuilder.h"
#include "Input.h"

#include <SDL2/SDL.h>

Scene::Scene(IRenderer* r) : renderer(r), lastTicks(SDL_GetTicks()) {}

void Scene::AddGameObject(std::unique_ptr<GameObject> go) {
    go->SetScene(this);
    gameObjectLookup[go->GetName()] = go.get();

    auto renderComponent = go->GetComponent<RendererComponent>();
    if (renderComponent != nullptr) {
        renderComponent->SetRenderer(renderer);
        gameObjectsRenderers.insert({ renderComponent->GetRenderOrder(), renderComponent });
    }

    auto collider = go->GetComponent<SimpleCollider>();
    if (collider != nullptr) {
        colliders.push_back(collider);
    }

    go->Start();
    gameObjects.push_back(std::move(go));
}

GameObject* Scene::FindGameObjectByName(const std::string& name) {
    auto it = gameObjectLookup.find(name);
    return it != gameObjectLookup.end() ? it->second : nullptr;
}

void Scene::CheckCollisions() {
    for (size_t i = 0; i < colliders.size(); ++i) {
        for (size_t j = i + 1; j < colliders.size(); ++j) {
            if (colliders[i]->IsColliding(*colliders[j])) {
                colliders[i]->GetGameObject()->OnCollide(colliders[j]->GetGameObject());
                colliders[j]->GetGameObject()->OnCollide(colliders[i]->GetGameObject());
            }
        }
    }
}

void Scene::Update(float deltaTime) {
    for (auto& go : gameObjects) go->Update(deltaTime);
}

void Scene::Render() {
    for (const auto& [_, component] : gameObjectsRenderers) {
        if (component->IsActive()) {
            component->Render();
        }
    }
    renderer->Present();
}

void Scene::Clear() {
    gameObjects.clear();
    gameObjectLookup.clear();
    gameObjectsRenderers.clear();
    colliders.clear();
}

bool Scene::IsRunning() const {
    return running;
}

void Scene::Stop() {
    running = false;
}

bool Scene::MainLoop() {
    const unsigned currentTicks = SDL_GetTicks();
    float deltaTime = (currentTicks - lastTicks) / 1000.0f;
    lastTicks = currentTicks;

    renderer->Clear();

    inputHandler.Update(deltaTime);

    if (inputHandler.WasQuitRequested())
    {
        Stop();
    }

    Update(deltaTime);
    CheckCollisions();
    Render();

    return IsRunning();
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const {
    return gameObjects;
}

const IRenderer* Scene::GetRenderer() const {
    return renderer;
}

GameObjectBuilder Scene::CreateGameObjectBuilder(const std::string& name, unsigned tag)
{
    return GameObjectBuilder(this, name, tag);
}

const Input& Scene::GetInput() const
{
    return inputHandler;
}