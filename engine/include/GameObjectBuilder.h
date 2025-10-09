#pragma once

#include "Std.h"

#include "GameObject.h" // Assuming this includes BaseComponent, etc.
#include "Components/Transform.h"

// Forward declare if needed to minimize includes
class Scene;

class GameObjectBuilder
{
public:
    GameObjectBuilder(Scene *scene, const String &name, unsigned tag)
        : scene(scene), go(MakeUnique<GameObject>(name, tag)) {}

    GameObjectBuilder(GameObject *parent, const String &name, unsigned tag)
        : parent(parent), go(MakeUnique<GameObject>(name, tag)) {}

    template <typename T, typename... Args>
    GameObjectBuilder &WithComponent(Args &&...args)
    {
        go->AddComponent<T>(std::forward<Args>(args)...);
        return *this;
    }

    template <typename T>
    GameObjectBuilder &WithComponent()
    {
        go->AddComponent<T>();
        return *this;
    }

    // TODO:
    GameObjectBuilder &WithChild(const String &childName, unsigned childTag)
    {
		assert(false && "WithChild is not implemented.");
        return *this;
    }

    GameObjectBuilder &AtPosition(float x, float y)
    {
        if (auto transform = go->GetTransform())
        {
            transform->SetPosition(x, y);
        }
        else
        {
			assert(false && "No transform component attached.");
        }
        return *this;
    }

    GameObject *AddToScene();

private:
    GameObject *parent = nullptr;
    Scene *scene = nullptr;
    UniquePtr<GameObject> go; // Temporary ownership during build
};