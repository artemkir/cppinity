#include "Components/Transform.h"
#include "Components/Canvas.h"
#include "GameObject.h"

Canvas* ScreenTransform::GetAncestorCanvas() const {
    auto canvas = gameObject->GetComponent<Canvas>();

	if (canvas) {
        return canvas;
    }

    if (parent) {
        return parent->GetAncestorCanvas();
    }
    return nullptr;
}

BaseTransform::Transform ScreenTransform::GetRelativeTransformToAncestor(const ScreenTransform* ancestor) const {
    Transform rel = GetScreenTransform(); // Local transform

    const ScreenTransform* current = this;
    while (current != ancestor && current->parent) {
        current = current->parent;
        Transform p_local = current->GetScreenTransform();
        rel.pos = rel.pos * p_local.scale + p_local.pos;
        rel.size = rel.size * p_local.scale; // Size scales with parent
        rel.scale = rel.scale * p_local.scale;
    }
    rel.size = rel.size * rel.scale; // Apply accumulated scale to size
    rel.scale = { 1.0f, 1.0f }; // Reset scale as it's applied

    return rel;
}

BaseTransform::Transform ScreenTransform::GetFinalScreenTransform() const {
    Canvas* canvas = GetAncestorCanvas();
    if (!canvas) {
        // No canvas: accumulate hierarchy to root
        if (!parent) {
            return GetScreenTransform(); // Local if no parent
        }
        return GetRelativeTransformToAncestor(nullptr);
    }
    else {
        // Has canvas: accumulate relative to canvas
        const ScreenTransform* canvas_transform = dynamic_cast<ScreenTransform*>(canvas->GetGameObject()->GetTransform());
        Transform rel = GetRelativeTransformToAncestor(canvas_transform);

        // Get canvas's screen position and size (viewport relative)
        Vector2 canvas_screen_pos = canvas->GetScreenPos();
        Vector2 canvas_screen_size = canvas->GetScreenSize();

        // Compute scale factor from canvas virtual size to screen size
        Vector2 canvas_scale = canvas_screen_size / canvas->GetCanvasSize();

        // Map relative (canvas-space) transform to screen
        Vector2 final_pos = canvas_screen_pos + rel.pos * canvas_scale;
        Vector2 final_size = rel.size * canvas_scale;

        return { final_pos, final_size, {1.0f, 1.0f} };
    }
}

BaseTransform::Transform GridTransform::GetFinalScreenTransform() const {
    // Reuse ScreenTransform logic, as GetScreenTransform has grid adjustment
    return ScreenTransform::GetFinalScreenTransform();
}