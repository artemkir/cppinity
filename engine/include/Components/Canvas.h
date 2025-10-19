// Canvas.h
#pragma once

#ifndef CANVAS_H
#define CANVAS_H

#include "Components/BaseComponent.h"
#include "Components/Transform.h"

enum class AnchorType {
        TopLeft,
        Center
};

class Canvas : public BaseComponent {
public:
   

    // Size in relative viewport coords (0 to 1, e.g., 1.0 = 100% screen width/height)
    // Position is anchor point (0 to 1, e.g., 0.5, 0.5 = centered)
    // anchor_type: "top_left", "center", etc., affects how pos is interpreted
    Canvas(const Vector2& relative_size, const Vector2& relative_pos, AnchorType anchor_type, const Vector2& canvas_size);

    const Vector2& GetCanvasSize() const; // Virtual resolution (e.g., 1920x1080)
    Vector2 GetRelativeSize() const; // Relative to viewport (0 to 1)
    Vector2 GetRelativePos() const; // Anchor point (0 to 1)
    AnchorType GetAnchorType() const;
    Vector2 GetScreenSize() const; // Pixel size based on screen
    Vector2 GetScreenPos() const; // Pixel pos based on anchor and screen

private:
    Vector2 canvas_size_; // Virtual resolution
    Vector2 relative_size_; // e.g., {1.0f, 1.0f} for 100% width/height
    Vector2 relative_pos_; // e.g., {0.5f, 0.5f} for centered
    AnchorType anchor_type_; // e.g., AnchorType::Center
};

#endif // CANVAS_H