#include "GameObject.h"
#include "Scene.h"
#include "Components/Transform.h"

GameObject::GameObject(const std::string& name_, unsigned tag_)
    : name(name_), tag(tag_) {}

void GameObject::SetScene(Scene* s) {
    scene = s;
}

Scene* GameObject::GetScene() {
    return scene;
}

void GameObject::SetActive(bool active) {
    for (const auto& comp : GetComponents()) {
        comp->SetActive(active);
    }

    for (const auto child : children) {
        child->SetActive(active);
    }
}

void GameObject::AddGameObject(std::unique_ptr<GameObject> go) {
    children.push_back(go.get());
    scene->AddGameObject(std::move(go));
}

void GameObject::AddComponent(std::unique_ptr<BaseComponent> component) {
    component->SetGameObject(this);

    if (const auto t = dynamic_cast<TileTransform*>(component.get())) {
        transform = t;
    }

    components.push_back(std::move(component));
}

void GameObject::Start() const
{
    for (const auto& comp : components) {
        if (comp->IsActive()) {
            comp->Start();
        }
    }
}

void GameObject::Update(float deltaTime) const
{
    for (const auto& comp : components) {
        if (comp->IsActive()) {
            comp->Update(deltaTime);
        }
    }
}

void GameObject::OnCollide(GameObject* other) const
{
    for (const auto& comp : components) {
        if (comp->IsActive()) {
            comp->OnCollide(other);
        }
    }
}

const std::string& GameObject::GetName() const {
    return name;
}

int GameObject::GetTag() const {
    return tag;
}

TileTransform* GameObject::GetTransform() const {
    return transform;
}

std::vector<std::unique_ptr<BaseComponent>>& GameObject::GetComponents() {
    return components;
}