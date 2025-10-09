// GameObject.cpp (unchanged, no SDL dependencies)
#include <stdexcept>

#include "GameObject.h"
#include "Scene.h"
#include "Components/Transform.h"
#include "GameObjectBuilder.h"

GameObject::GameObject(const String &name_, unsigned tag_)
    : name(name_), tag(tag_) {}

void GameObject::SetScene(Scene *s)
{
    scene = s;
}

Scene *GameObject::GetScene()
{
    return scene;
}

void GameObject::SetActive(bool active)
{
    for (const auto &comp : GetComponents())
    {
        comp->SetActive(active);
    }

    for (const auto child : children)
    {
        child->SetActive(active);
    }
}

void GameObject::AddGameObject(UniquePtr<GameObject> go)
{
    children.push_back(go.get());
    go->parent = this;
    scene->AddGameObject(std::move(go));
}

void GameObject::AddComponent(UniquePtr<BaseComponent> component)
{
    component->SetGameObject(this);

    if (const auto t = dynamic_cast<BaseTransform *>(component.get()))
    {
        transform = t;
    }

    components.push_back(std::move(component));
}

template <typename Func>
inline void GameObject::ForEachActiveComponent(Func f) const
{
    for (const auto &comp : components)
    {
        if (comp->IsActive())
        {
            f(comp);
        }
    }
}

void GameObject::Awake() const
{
    ForEachActiveComponent([](const auto &comp)
                           { comp->Awake(); });
}

void GameObject::Start()
{
    ForEachActiveComponent([](const auto &comp)
                           { comp->Start(); });
    isStarted = true;
}

void GameObject::Update(float deltaTime) const
{
    ForEachActiveComponent([deltaTime](const auto &comp)
                           { comp->Update(deltaTime); });
}

void GameObject::OnCollide(GameObject *other) const
{
    ForEachActiveComponent([other](const auto &comp)
                           { comp->OnCollide(other); });
}

const String &GameObject::GetName() const
{
    return name;
}

int GameObject::GetTag() const
{
    return tag;
}

BaseTransform *GameObject::GetTransform() const
{
    return transform;
}

Vector<UniquePtr<BaseComponent>> &GameObject::GetComponents()
{
    return components;
}

bool GameObject::IsAncestorOf(const String &name, unsigned tag) const
{
    const GameObject *current = this;
    while (current)
    {
        if (current->GetName() == name && current->GetTag() == tag)
            return true;

        current = current->parent;
    }
    return false;
}

GameObjectBuilder GameObject::CreateGameObjectBuilder(const String &name_, unsigned tag_)
{
    if (IsAncestorOf(name_, tag_))
    {
        throw std::runtime_error("Cycle detected in parent-child hierarchy: cannot create child with name '" + name_ + "' and tag " + std::to_string(tag_));
    }

    return GameObjectBuilder(this, name_, tag_);
}