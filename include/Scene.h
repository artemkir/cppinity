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

// Scene Class
class Scene {
	std::vector<std::unique_ptr<GameObject>> gameObjects;  // preserves order 
	std::unordered_map<std::string, GameObject*> gameObjectLookup;  // fast lookup
	std::multimap<int, RendererComponent*> gameObjectsRenderers;
	std::vector<SimpleCollider*> colliders;

	IRenderer* renderer;

	void CheckCollisions();

	Input inputHandler;

public:
	Scene(IRenderer* r);

	void AddGameObject(std::unique_ptr<GameObject> go);

	GameObject* FindGameObjectByName(const std::string& name);
	
	void Update(float deltaTime);

	void Render();

	void Clear();

	void Stop();

	void Frame(float deltaTime);  // Main frame logic for Sokol

	void HandleEvent(const sapp_event* e);  // Handle Sokol events

	const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;
	const IRenderer* GetRenderer() const;
		
	GameObjectBuilder CreateGameObjectBuilder(const std::string& name, unsigned tag = 0);

	const Input& GetInput() const;
	
};