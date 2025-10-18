#include "Engine.h"

Canvas::Canvas(const Vector2& relative_size, const Vector2& relative_pos, const String& anchor_type, const Vector2& canvas_size)
    : relative_size_(relative_size), relative_pos_(relative_pos), anchor_type_(anchor_type), canvas_size_(canvas_size) {
}

const Vector2& Canvas::GetCanvasSize() const {
    return canvas_size_;
}

Vector2 Canvas::GetRelativeSize() const {
    return relative_size_;
}

Vector2 Canvas::GetRelativePos() const {
    return relative_pos_;
}

String Canvas::GetAnchorType() const {
    return anchor_type_;
}

Vector2 Canvas::GetScreenSize() const {
    float sw = static_cast<float>(gameObject->GetScene()->GetRenderer()->GetW());
    float sh = static_cast<float>(gameObject->GetScene()->GetRenderer()->GetH());

    // Compute size based on relative values
    Vector2 size = relative_size_;
    // Special case: if width is -1, set equal to height (e.g., square canvas)
    if (relative_size_.x < 0.0f) {
        size.x = relative_size_.y * (sh / sw); // Maintain aspect
    }
    return Vector2{ size.x * sw, size.y * sh }; // Convert to pixels
}

Vector2 Canvas::GetScreenPos() const {
    float sw = static_cast<float>(gameObject->GetScene()->GetRenderer()->GetW());
    float sh = static_cast<float>(gameObject->GetScene()->GetRenderer()->GetH());

    Vector2 screen_size = GetScreenSize();
    Vector2 pos = relative_pos_;

    if (anchor_type_ == "center") {
        // Center: adjust pos so anchor is at relative_pos
        pos = pos * Vector2{ sw, sh } - screen_size * 0.5f;
    }
    else {
        // Default: top-left anchor
        pos = pos * Vector2{ sw, sh };
    }
    return pos;
}