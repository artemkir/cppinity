#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

#include "GameObject.h"
#include "Input.h"

class RendererComponent;
class IRenderer;
class SimpleCollider;
class GameObjectBuilder;

// Scene Class
class Scene {
	std::vector<std::unique_ptr<GameObject>> gameObjects;  // preserves order 
	std::unordered_map<std::string, GameObject*> gameObjectLookup;  // fast lookup
	std::multimap<int, RendererComponent*> gameObjectsRenderers;
	std::vector<SimpleCollider*> colliders;

	IRenderer* renderer;
	bool running = true;
	unsigned lastTicks = 0;  // For calculating delta time

	void CheckCollisions();

	Input inputHandler;

public:
	Scene(IRenderer* r);

	void AddGameObject(std::unique_ptr<GameObject> go);

	GameObject* FindGameObjectByName(const std::string& name);
	
	void Update(float deltaTime);

	void Render();

	void Clear();

	bool IsRunning() const;
	void Stop();

	bool MainLoop();

	const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;
	const IRenderer* GetRenderer() const;
		
	GameObjectBuilder CreateGameObjectBuilder(const std::string& name, unsigned tag);

	const Input& GetInput() const;
	
};

