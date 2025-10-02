// Scene.h
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
struct InputEvent;

// Scene Class
class Scene {
	std::vector<std::unique_ptr<GameObject>> gameObjects;  // preserves order 
	std::unordered_map<std::string, GameObject*> gameObjectLookup;  // fast lookup
	std::multimap<int, RendererComponent*> gameObjectsRenderers;
	std::vector<SimpleCollider*> colliders;

	IRenderer* renderer;
	bool running = true;

	void CheckCollisions();

	Input inputHandler;

public:
	Scene(IRenderer* r);

	void AddGameObject(std::unique_ptr<GameObject> go);

	GameObject* FindGameObjectByName(const std::string& name);
	
	void Update(float deltaTime);

	void Render();

	void BeginPass();

	bool IsRunning() const;

	void Stop();

	bool Frame(float deltaTime); 

	void HandleEvent(const InputEvent* event);  

	const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;
	const IRenderer* GetRenderer() const;
		
	GameObjectBuilder CreateGameObjectBuilder(const std::string& name, unsigned tag = 0);

	const Input& GetInput() const;
	
};