#include "Components/Transform.h"
#include "Components/Canvas.h"
#include "GameObject.h"

void ScreenTransform::Awake() {
    if (gameObject->GetParent())
    {
        parent = gameObject->GetParent()->GetComponent<ScreenTransform>();
    }
    canvas = GetAncestorCanvas();
}

Canvas* ScreenTransform::GetAncestorCanvas() const {
    auto canvas = gameObject->GetComponent<Canvas>();
    if (canvas) {
        return canvas;
    }
    if (parent) {
        return parent->GetAncestorCanvas();
    }
    if (gameObject->GetParent())
    {
        canvas = gameObject->GetParent()->GetComponent<Canvas>();
    }
    return canvas;
}

BaseTransform::Transform ScreenTransform::GetScreenTransform() const {
    Vector2 scaledSize = GetSize() * GetScale();
    Vector2 pivotOffset = scaledSize * GetPivot();
    Vector2 adjustedPos = GetPos() - pivotOffset;
    return { adjustedPos, GetSize(), GetScale(), GetPivot() }; // Return unscaled size
}

BaseTransform::Transform ScreenTransform::GetRelativeTransformToAncestor(const ScreenTransform* ancestor) const {
    Transform rel = GetScreenTransform(); // Local transform with unscaled size

    const ScreenTransform* current = this;
    while (current != ancestor && current->parent) {
        current = current->parent;
        Transform p_local = current->GetScreenTransform();
        Vector2 parentPivotOffset = p_local.size * p_local.scale * p_local.pivot;
        rel.pos = rel.pos * p_local.scale + p_local.pos - parentPivotOffset;
        rel.size = rel.size * p_local.scale; // Apply parent scale to size
        rel.scale = rel.scale * p_local.scale; // Accumulate scales
    }
    // Apply only the object's own scale to its size, as parent scales are already applied
    Transform local = GetScreenTransform();
    rel.size = local.size * rel.scale; // Use local size with accumulated scale
    rel.scale = { 1.0f, 1.0f }; // Reset scale as it's now applied to size
    return rel;
}

BaseTransform::Transform ScreenTransform::GetFinalScreenTransform() const {
    if (!canvas) 
    {
        if (!parent) 
        {
            return GetScreenTransform();
        }
    
        return GetRelativeTransformToAncestor(nullptr);
    }
    else {
        Transform rel = GetRelativeTransformToAncestor(nullptr);

        Vector2 canvas_screen_pos = canvas->GetScreenPos();
        Vector2 canvas_screen_size = canvas->GetScreenSize();
        Vector2 canvas_scale = canvas_screen_size / canvas->GetCanvasSize();

        Vector2 final_pos = canvas_screen_pos + rel.pos * canvas_scale;
        Vector2 final_size = rel.size * canvas_scale;

        return { final_pos, final_size, {1.0f, 1.0f}, rel.pivot };
    }
}

BaseTransform::Transform GridTransform::GetFinalScreenTransform() const {
    return ScreenTransform::GetFinalScreenTransform();
}