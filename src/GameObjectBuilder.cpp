#include "GameObjectBuilder.h"
#include "Scene.h"

// Finalize and add to scene

GameObject* GameObjectBuilder::AddToScene() {
    
    GameObject* ptr = go.get();
    
    if (parent) {
        parent->AddGameObject(std::move(go));  // Add as child
    }
    else if (scene) {
        scene->AddGameObject(std::move(go));  // Add to scene
    }
    else {
        // Error: No parent or scene provided
    }
    
    return ptr;
}
