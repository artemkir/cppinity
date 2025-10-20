#pragma once

#include "Std.h"

#include "Components/BaseComponent.h"

// Forward declaration
class Scene;
class BaseTransform;
class GameObjectBuilder;

// GameObject Class
class GameObject
{
    Scene *scene = nullptr;
    unsigned tag;
    String name;
    List<UniquePtr<BaseComponent>> components;
    BaseTransform *transform = nullptr;
    List<GameObject *> children;
    GameObject *parent = nullptr;
    bool isStarted = false;

public:
    // GameObject() = delete;

    explicit GameObject(const String &name_, unsigned tag_ = 0);

    void SetScene(Scene *s);
    Scene *GetScene();

	GameObject* GetParent() const { return parent; }
	const List<GameObject*>& GetChildren() const { return children; }

    void RemoveMeFromParent()
    {
        if (parent)
        {
            auto &siblings = parent->children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
            parent = nullptr;
		}
    }

    void SetActive(bool active);
    void AddGameObject(UniquePtr<GameObject> go);
    void AddComponent(UniquePtr<BaseComponent> component);

    bool HasStarted() const { return isStarted; }

    template <typename T>
    T *GetComponent()
    {
        for (const auto &comp : components)
        {
            if (auto casted = dynamic_cast<T *>(comp.get()))
                return casted;
        }
        return nullptr;
    }

    void Awake() const;
    void Start();
    void Update(float deltaTime) const;
    void OnCollide(GameObject *other) const;
    void OnDestroy() const;

    const String &GetName() const;
    int GetTag() const;
    BaseTransform *GetTransform() const;
    List<UniquePtr<BaseComponent>> &GetComponents();

    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        static_assert(std::is_base_of_v<BaseComponent, T>, "T must derive from BaseComponent");

        auto component = MakeUnique<T>(std::forward<Args>(args)...);
        T *ptr = component.get();

        AddComponent(std::move(component));

        return ptr;
    }

    template <typename Func>
    void ForEachActiveComponent(Func f) const;

    bool IsAncestorOf(const String &name, unsigned tag) const;
    GameObjectBuilder CreateGameObjectBuilder(const String &name, unsigned tag);
};
