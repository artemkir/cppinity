#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Components/BaseComponent.h"

// Forward declaration
class Scene;
class TileTransform;

// GameObject Class
class GameObject {
    Scene* scene = nullptr;
    unsigned tag;
    std::string name;
    std::vector<std::unique_ptr<BaseComponent>> components;
    TileTransform* transform = nullptr;
    std::vector<GameObject*> children;

public:
    //GameObject() = delete;

    GameObject(const std::string& name_, unsigned tag_ = 0);

    void SetScene(Scene* s);
    Scene* GetScene();

    void SetActive(bool active);
    void AddGameObject(std::unique_ptr<GameObject> go);
    void AddComponent(std::unique_ptr<BaseComponent> component);

    template<typename T>
    T* GetComponent() {
        for (const auto& comp : components) {
            if (T* casted = dynamic_cast<T*>(comp.get()))
                return casted;
        }
        return nullptr;
    }

    void Start() const;
    void Update(float deltaTime) const;
    void OnCollide(GameObject* other) const;

    const std::string& GetName() const;
    int GetTag() const;
    TileTransform* GetTransform() const;
    std::vector<std::unique_ptr<BaseComponent>>& GetComponents();
};
