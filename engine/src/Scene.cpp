// Scene.cpp (updated: remove SDL, update for Sokol timing/input integration)
#include "Scene.h"
#include "GameObject.h"
#include "Components/RendererComponent.h"
#include "Components/SimpleCollider.h"
#include "IRenderer.h"
#include "GameObjectBuilder.h"
#include "MaterialManager.h"
#include "ResourceManager.h"
#include "Input.h"

Scene::Scene(IRenderer *r, MaterialManager *m, ResourceManager *resourceManager) : renderer(r), materialManager(m), resourceManager(resourceManager) {}

void Scene::AddGameObject(UniquePtr<GameObject> go, bool immediate)
{
    if (!immediate)
    {
        pendingAdd.push_back(std::move(go));
        return;
    }

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

void Scene::Destroy(GameObject* go) {
    if (go->GetScene() != this) {
        return;
    }

    if (std::find(pendingDestroy.begin(), pendingDestroy.end(), go) != pendingDestroy.end()) {
        return;
    }

    pendingDestroy.push_back(go);
}

void Scene::DestroyImmediate(GameObject* go)
{
    if (!go) return;

    // Remove from parent's children list if it has a parent
    if (auto parent = go->GetParent())
    {
        go->RemoveMeFromParent();
    }
        
    // Recursively destroy all children first
    List<GameObject*> childrenCopy = go->GetChildren();
    for (auto child : childrenCopy)
    {
        if (!child)
            continue;
        
        DestroyImmediate(child);
    }
    // At this point, go->children should be empty due to recursive calls

    // Remove renderer component from gameObjectsRenderers if present
    if (auto renderComp = go->GetComponent<RendererComponent>(); renderComp)
    {
        for (auto it = gameObjectsRenderers.begin(); it != gameObjectsRenderers.end(); )
        {
            if (it->second == renderComp)
            {
                it = gameObjectsRenderers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // Remove collider from colliders if present
    if (auto coll = go->GetComponent<SimpleCollider>(); coll)
    {
        auto& cols = colliders;
        cols.erase(std::remove(cols.begin(), cols.end(), coll), cols.end());
    }

    // Remove from gameObjectLookup
    gameObjectLookup.erase(go->GetName());

    // Remove from gameObjects (this will delete the GameObject via unique_ptr)
    auto& gos = gameObjects;
    for (auto it = gos.begin(); it != gos.end(); ++it)
    {
        if (it->get() == go)
        {
             gos.erase(it);
            break;
        }
    }
}

void Scene::ProcessPendingDestroys() 
{
    while (!pendingDestroy.empty()) 
    {
        GameObject* goPtr = pendingDestroy.back();
        pendingDestroy.pop_back();

        if (goPtr && goPtr->GetScene() == this) 
        {
            DestroyImmediate(goPtr);
        }
    }
}

void Scene::ProcessPendingAdds()
{
    while (!pendingAdd.empty())
    {
        AddGameObject(std::move(pendingAdd.back()), true);
        pendingAdd.pop_back();
    }
}

GameObject *Scene::FindGameObjectByName(const String &name)
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

    ProcessPendingDestroys();
    ProcessPendingAdds();

    renderer->EndPass();

    return IsRunning();
}

void Scene::HandleEvent(const InputEvent *event)
{
    inputHandler.HandleEvent(event);
}

GameObjectBuilder Scene::CreateGameObjectBuilder(const String &name, unsigned tag)
{
    return GameObjectBuilder(this, name, tag);
}

const Input &Scene::GetInput() const
{
    return inputHandler;
}