// Scene.h
#pragma once

#include "Std.h"

#include "GameObject.h"
#include "Input.h"

class RendererComponent;
class IRenderer;
class SimpleCollider;
class GameObjectBuilder;
struct InputEvent;
class MaterialManager;
class ResourceManager;

// Scene Class
class Scene
{
	Vector<UniquePtr<GameObject>> gameObjects;			// preserves order
	UnorderedMapStringKey<GameObject *> gameObjectLookup; // fast lookup
	MultiMap<int, RendererComponent *> gameObjectsRenderers;
	Vector<SimpleCollider *> colliders;

	IRenderer *renderer;
	MaterialManager *materialManager;
	ResourceManager *resourceManager;

	bool running = true;

	void CheckCollisions();

	Input inputHandler;

public:
	Scene(IRenderer *r, MaterialManager *m, ResourceManager *tm);

	void AddGameObject(UniquePtr<GameObject> go);

	GameObject *FindGameObjectByName(const String &name);

	void Update(float deltaTime);

	void Render();

	void BeginPass();

	bool IsRunning() const;

	void Stop();

	bool Frame(float deltaTime);

	void HandleEvent(const InputEvent *event);

	const Vector<UniquePtr<GameObject>>& Scene::GetGameObjects() const { return gameObjects;	}
	
	const IRenderer* Scene::GetRenderer() const { return renderer;	}

	const MaterialManager* Scene::GetMaterialManager() const {	return materialManager;	}

	ResourceManager* GetResourceManager() const { return resourceManager; };

	GameObjectBuilder CreateGameObjectBuilder(const String &name, unsigned tag = 0);

	const Input &GetInput() const;
};