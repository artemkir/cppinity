#pragma once

#include <memory>  // For unique_ptr
#include <string>

#include "GameObject.h"  // Assuming this includes BaseComponent, etc.
#include "Components/Transform.h"

// Forward declare if needed to minimize includes
class Scene;

class GameObjectBuilder {
public:
  
    GameObjectBuilder(Scene* scene, const std::string& name, unsigned tag)
        : scene(scene), go(std::make_unique<GameObject>(name, tag)) {}

    GameObjectBuilder(GameObject* parent, const std::string& name, unsigned tag)
        : parent(parent), scene(nullptr), go(std::make_unique<GameObject>(name, tag)) {}

    template <typename T, typename... Args>
    GameObjectBuilder& WithComponent(Args&&... args) {
        go->AddComponent<T>(std::forward<Args>(args)...);
        return *this;
    }

    template <typename T>
    GameObjectBuilder& WithComponent() {
        go->AddComponent<T>();
        return *this;
    }

    //TODO:
    GameObjectBuilder& WithChild(const std::string& childName, unsigned childTag) {
        //go_->AddChild(childName, childTag); 
        throw std::runtime_error("WithChild is not implemented.");
        return *this;
    }

    GameObjectBuilder& AtPosition(float x, float y) {
        if (auto transform = go->GetTransform()) {
            transform->SetPosition(x, y);
        }
        else {
            throw std::runtime_error("No transform component attached.");
        }
        return *this;
    }

    GameObject* AddToScene();

private:
    GameObject* parent = nullptr;
    Scene* scene = nullptr;  // Non-owning; assume scene outlives builder
    std::unique_ptr<GameObject> go;  // Temporary ownership during build
};