#pragma once

#include "ComponentFactory.h"

class SceneLoader
{
public:
    explicit SceneLoader(ComponentFactory &componentFactory_) : ComponentFactory(componentFactory_) {}

    void LoadScene(Scene *scene, const std::vector<GameObjectData> &data)
    {
        for (const auto &objData : data)
        {
            auto gameObject = std::make_unique<GameObject>(objData.name, objData.tag);
            for (const auto &compData : objData.components)
            {
                auto component = componentFactory.Create(compData);
                gameObject->AddComponent(std::move(component));
            }
            scene->AddGameObject(std::move(gameObject));
        }
    }

private:
    ComponentFactory &componentFactory;
};