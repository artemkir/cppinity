// Input.h
#pragma once

#include "Std.h"

#include "Components/BaseComponent.h"
#include "Components/Transform.h"
#include "GameObject.h"

class Canvas : public BaseComponent {
public:
    // canvas_size defines the internal virtual pixel resolution (e.g., 1920x1080).
    // The canvas's Transform defines its screen rect (position and size on screen).
    // Children use coordinates in [0, canvas_size.x] x [0, canvas_size.y].
    Canvas(const Vector2& canvas_size);

    const Vector2& GetCanvasSize() const {
		return canvas_size_;
    }

private:
    Vector2 canvas_size_;
};