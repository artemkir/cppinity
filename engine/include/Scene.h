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
	List<UniquePtr<GameObject>> gameObjects;			// preserves order
	UnorderedMapStringKey<GameObject *> gameObjectLookup; // fast lookup
	MultiMap<int, RendererComponent *> gameObjectsRenderers;
	List<SimpleCollider *> colliders;
	List<GameObject*> pendingDestroy;
	List<UniquePtr<GameObject>> pendingAdd;

	IRenderer *renderer;
	MaterialManager *materialManager;
	ResourceManager *resourceManager;

	bool running = true;

	void CheckCollisions();

	Input inputHandler;

	void DestroyImmediate(GameObject* go);
	void ProcessPendingDestroys();
	void ProcessPendingAdds();

public:
	Scene(IRenderer *r, MaterialManager *m, ResourceManager *tm);

	void AddGameObject(UniquePtr<GameObject> go, bool immediate);

	void Destroy(GameObject* go);
	
	
	GameObject *FindGameObjectByName(const String &name);

	void Update(float deltaTime);

	void Render();

	void BeginPass();

	bool IsRunning() const;

	void Stop();

	bool Frame(float deltaTime);

	void HandleEvent(const InputEvent *event);

	const List<UniquePtr<GameObject>>& GetGameObjects() const { return gameObjects;	}
	
	const IRenderer* GetRenderer() const { return renderer;	}

	const MaterialManager* GetMaterialManager() const {	return materialManager;	}

	ResourceManager* GetResourceManager() const { return resourceManager; };

	GameObjectBuilder CreateGameObjectBuilder(const String &name, unsigned tag = 0);

	const Input &GetInput() const;
};