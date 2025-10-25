#include <stdexcept>
#include <iostream>

#include "GameObjectBuilder.h"
#include "Scene.h"

// Finalize and add to scene
GameObject *GameObjectBuilder::AddToScene()
{
    GameObject *ptr = go.get();

    if (parent)
    {
        parent->AddGameObject(std::move(go)); // Add as child
        return ptr;
    }
    else if (scene)
    {
        scene->AddGameObject(std::move(go), false); // Add to scene
        return ptr;
    }
        
    throw std::runtime_error("GameObjectBuilder::AddToScene: No parent or scene provided.");
}
