#pragma once

//#include "Scene.h"

class Scene;

// GameObject Class
class GameObject {

	Scene* scene = nullptr;

	unsigned tag;
	std::string name;
	std::vector<std::unique_ptr<BaseComponent>> components;

	TileTransform* transform = nullptr;  // Cached pointer to Transform component
	std::vector<GameObject*> children;

public:
	GameObject(const std::string& name_, unsigned tag_ = 0)
		: name(name_), tag(tag_) {}

	void SetScene(Scene* s) { scene = s; }
	Scene* GetScene() { return scene; }

	std::string GetName() { return name; }

	void SetActive(bool active) {
		for (const auto& comp : GetComponents()) {
			comp->SetActive(active); // Enable/disable by nulling GameObject reference
		}

		for (auto child : children)
		{
			child->SetActive(active);
		}
	}

	void AddGameObject(std::unique_ptr<GameObject> go)
	{
		children.push_back(go.get());

		scene->AddGameObject(std::move(go));
	}

	void AddComponent(std::unique_ptr<BaseComponent> component) {
		component->SetGameObject(this);

		if (auto t = dynamic_cast<TileTransform*>(component.get())) {
			transform = t;
		}

		components.push_back(std::move(component));
	}

	template<typename T>
	T* GetComponent() {
		for (const auto& comp : components) {
			if (T* casted = dynamic_cast<T*>(comp.get()))
				return casted;
		}
		return nullptr;
	}

	void Start() {
		for (const auto& comp : components)
		{
			if (comp->IsActive())
			{
				comp->Start();
			}
		}
	}

	void Update(float deltaTime) {
		for (const auto& comp : components)
		{
			if (comp->IsActive())
			{
				comp->Update(deltaTime);
			}
		}
	}

	void OnCollide(GameObject* other) {
		for (const auto& comp : components)
		{
			if (comp->IsActive())
			{
				comp->OnCollide(other);
			}
		}
	}

	const std::string& GetName() const { return name; }
	const int GetTag() const { return tag; }

	TileTransform* GetTransform() const { return transform; }
	std::vector<std::unique_ptr<BaseComponent>>& GetComponents() { return components; }
};