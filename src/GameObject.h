#pragma once

class Scene;

// GameObject Class
class GameObject {

	Scene* scene = nullptr;

	unsigned tag;
	std::string name;
	std::vector<std::unique_ptr<BaseComponent>> components;

	TileTransform* transform = nullptr;  // Cached pointer to Transform component

public:
	GameObject(const std::string& name_, unsigned tag_ = 0)
		: name(name_), tag(tag_) {}

	void SetScene(Scene* s) { scene = s; }
	Scene* GetScene() { return scene; }

	std::string GetName() { return name; }

	void AddComponent(std::unique_ptr<BaseComponent> component) {
		component->SetGameObject(this);

		if (auto t = dynamic_cast<TileTransform*>(component.get())) {
			transform = t;
		}

		components.push_back(std::move(component));
	}

	template<typename T>
	T* GetComponent() {
		for (auto& comp : components) {
			if (T* casted = dynamic_cast<T*>(comp.get()))
				return casted;
		}
		return nullptr;
	}

	void Start() {
		for (auto& comp : components) comp->Start();
	}

	void Update(float deltaTime) {
		for (auto& comp : components) comp->Update(deltaTime);
	}

	void OnCollide(GameObject* other) {
		for (auto& comp : components) comp->OnCollide(other);
	}

	const std::string& GetName() const { return name; }
	const int GetTag() const { return tag; }

	TileTransform* GetTransform() const { return transform; }
	std::vector<std::unique_ptr<BaseComponent>>& GetComponents() { return components; }
};