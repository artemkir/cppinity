#pragma once

#include <string>
#include <vector>
#include <memory>
// #include "GameObjectBuilder.h"
#include "Components/BaseComponent.h"

// Forward declaration
class Scene;
class TileTransform;
class GameObjectBuilder;

// GameObject Class
class GameObject
{
    Scene *scene = nullptr;
    unsigned tag;
    std::string name;
    std::vector<std::unique_ptr<BaseComponent>> components;
    TileTransform *transform = nullptr;
    std::vector<GameObject *> children;
    GameObject *parent = nullptr;
    bool isStarted = false;

public:
    // GameObject() = delete;

    GameObject(const std::string &name_, unsigned tag_ = 0);

    void SetScene(Scene *s);
    Scene *GetScene();

    void SetActive(bool active);
    void AddGameObject(std::unique_ptr<GameObject> go);
    void AddComponent(std::unique_ptr<BaseComponent> component);

    bool HasStarted() const { return isStarted; }

    template <typename T>
    T *GetComponent()
    {
        for (const auto &comp : components)
        {
            if (T *casted = dynamic_cast<T *>(comp.get()))
                return casted;
        }
        return nullptr;
    }

    void Awake() const;
    void Start();
    void Update(float deltaTime) const;
    void OnCollide(GameObject *other) const;

    const std::string &GetName() const;
    int GetTag() const;
    TileTransform *GetTransform() const;
    std::vector<std::unique_ptr<BaseComponent>> &GetComponents();

    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from BaseComponent");

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T *ptr = component.get();

        AddComponent(std::move(component));

        return ptr;
    }

    template <typename Func>
    void ForEachActiveComponent(Func f) const;

    bool IsAncestorOf(const std::string &name, unsigned tag) const;
    GameObjectBuilder CreateGameObjectBuilder(const std::string &name, unsigned tag);
};
