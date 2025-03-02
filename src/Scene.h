#pragma once

#include <map>

// Scene Class
class Scene {
private:
	std::vector<std::unique_ptr<GameObject>> gameObjects;  // preserves order 
	std::unordered_map<std::string, GameObject*> gameObjectLookup;  // fast lookup
	std::multimap<int, IRendererComponent*> gameObjectsRenderers;
	std::vector<SimpleCollider*> colliders;

	IRenderer& renderer;
	bool running = true;
	Uint32 lastTicks = 0;  // For calculating delta time

	void CheckCollisions() {
		for (size_t i = 0; i < colliders.size(); ++i) {
			for (size_t j = i + 1; j < colliders.size(); ++j) {
				if (colliders[i]->IsColliding(*colliders[j])) {
					colliders[i]->GetGameObject()->OnCollide(colliders[j]->GetGameObject());
					colliders[j]->GetGameObject()->OnCollide(colliders[i]->GetGameObject());
				}
			}
		}
	}

public:
	Scene(IRenderer& r) : renderer(r), lastTicks(SDL_GetTicks()) {}

	void AddGameObject(std::unique_ptr<GameObject> go) {
		go->SetScene(this);
		gameObjectLookup[go->GetName()] = go.get(); // store raw pointer for lookup

		auto renderer = go->GetComponent<IRendererComponent>();

		if (renderer != nullptr)
		{
			gameObjectsRenderers.insert({ renderer->Order(),renderer });
		}

		auto collider = go->GetComponent<SimpleCollider>();
		if (collider != nullptr)
		{
			colliders.push_back(collider);
		}

		go->Start();

		gameObjects.push_back(std::move(go));       // store unique_ptr in vector for ordered iteration
	}

	GameObject* FindGameObjectByName(const std::string& name) {
		auto it = gameObjectLookup.find(name);
		return it != gameObjectLookup.end() ? it->second : nullptr;
	}
	
	void Update(float deltaTime) {
		for (auto& go : gameObjects) go->Update(deltaTime);
	}

	void Render() {
		for (auto& [_, go] : gameObjectsRenderers) go->Render(renderer);

		renderer.Present();
	}

	bool IsRunning() const { return running; }
	void Stop() { running = false; }

	bool MainLoop() {
		Uint32 currentTicks = SDL_GetTicks();
		float deltaTime = (currentTicks - lastTicks) / 1000.0f;
		lastTicks = currentTicks;

		Update(deltaTime);
		CheckCollisions();
		Render();

		return IsRunning();
	}

	const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return gameObjects; }

};