// Scene.cpp (updated: remove SDL, update for Sokol timing/input integration)
#include "Scene.h"
#include "GameObject.h"
#include "Components/RendererComponent.h"
#include "Components/SimpleCollider.h"
#include "IRenderer.h"
#include "GameObjectBuilder.h"
#include "TexturesManager.h"
#include "MaterialManager.h"
#include "Input.h"

Scene::Scene(IRenderer *r, MaterialManager *m, TexturesManager *tm) : renderer(r), mm(m), tm(tm) {}

void Scene::AddGameObject(std::unique_ptr<GameObject> go)
{
    go->SetScene(this);
    gameObjectLookup[go->GetName()] = go.get();

    auto renderComponent = go->GetComponent<RendererComponent>();
    if (renderComponent != nullptr)
    {
        renderComponent->SetRenderer(renderer);
        gameObjectsRenderers.insert({renderComponent->GetRenderOrder(), renderComponent});
    }

    auto collider = go->GetComponent<SimpleCollider>();
    if (collider != nullptr)
    {
        colliders.push_back(collider);
    }

    go->Awake();
    gameObjects.push_back(std::move(go));
}

GameObject *Scene::FindGameObjectByName(const std::string &name)
{
    auto it = gameObjectLookup.find(name);
    return it != gameObjectLookup.end() ? it->second : nullptr;
}

void Scene::CheckCollisions()
{
    for (size_t i = 0; i < colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < colliders.size(); ++j)
        {
            if (colliders[i]->IsColliding(*colliders[j]))
            {
                colliders[i]->GetGameObject()->OnCollide(colliders[j]->GetGameObject());
                colliders[j]->GetGameObject()->OnCollide(colliders[i]->GetGameObject());
            }
        }
    }
}

void Scene::Update(float deltaTime)
{
    for (auto &go : gameObjects)
    {
        if (!go->HasStarted())
        {
            go->Start(); // called only once
        }

        go->Update(deltaTime);
    }
}

void Scene::Render()
{
    for (const auto &[_, component] : gameObjectsRenderers)
    {
        if (component->IsActive())
        {
            component->Render();
        }
    }
}

void Scene::BeginPass()
{
    gameObjects.clear();
    gameObjectLookup.clear();
    gameObjectsRenderers.clear();
    colliders.clear();
}

bool Scene::IsRunning() const
{
    return running;
}

void Scene::Stop()
{
    running = false;
}

bool Scene::Frame(float deltaTime)
{
    inputHandler.BeginFrame(); // Prepare input for frame (compute pressed/released)

    if (inputHandler.WasQuitRequested())
    {
        Stop();
    }

    renderer->BeginPass();

    Update(deltaTime);
    CheckCollisions();
    Render();

    renderer->EndPass();

    return IsRunning();
}

void Scene::HandleEvent(const InputEvent *event)
{
    inputHandler.HandleEvent(event);
}

const std::vector<std::unique_ptr<GameObject>> &Scene::GetGameObjects() const
{
    return gameObjects;
}

const IRenderer *Scene::GetRenderer() const
{
    return renderer;
}

const MaterialManager *Scene::GetMaterialManager() const
{
    return mm;
}

TexturesManager *Scene::GetTextureManager() const
{
    return tm;
}

GameObjectBuilder Scene::CreateGameObjectBuilder(const std::string &name, unsigned tag)
{
    return GameObjectBuilder(this, name, tag);
}

const Input &Scene::GetInput() const
{
    return inputHandler;
}