#pragma once

#include "Std.h"
#include "Math/Vector2.h"
#include "BaseComponent.h"

class Canvas;

class BaseTransform : public BaseComponent
{
public:
    struct Transform
    {
        Vector2 pos;
        Vector2 size;
        Vector2 scale;
        Vector2 pivot;
    };

    Transform t{ {0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f} };
    
    explicit BaseTransform(Vector2 pos, Vector2 size, Vector2 scale = Vector2{ 1.0f, 1.0f }, Vector2 pivot = Vector2{ 0.0f, 0.0f })
        : t{ pos, size, scale, pivot } {
    }

    const Vector2& GetPos() const { return t.pos; }
    const Vector2& GetSize() const { return t.size; }
    const Vector2& GetScale() const { return t.scale; }
    const Vector2& GetPivot() const { return t.pivot; } // New
    void SetSize(float width_, float height_) { t.size = { width_, height_ }; }
    void SetPosition(float x_, float y_) { t.pos = { x_, y_ }; }
    void SetScale(float x_, float y_) { t.scale = { x_, y_ }; }
    void SetPivot(float x_, float y_) { t.pivot = { x_, y_ }; } // New

    virtual Transform GetScreenTransform() const = 0;
    virtual Transform GetFinalScreenTransform() const = 0;
};

class ScreenTransform : public BaseTransform
{
    ScreenTransform* parent = nullptr;
    Canvas* canvas = nullptr;

    Canvas* GetAncestorCanvas() const;
    Transform GetRelativeTransformToAncestor(const ScreenTransform* ancestor) const;

public:
    using BaseTransform::BaseTransform;
  
    Transform GetScreenTransform() const override;
    Transform GetFinalScreenTransform() const override;

    void Awake() override;
};

class GridTransform : public ScreenTransform
{
public:
    using ScreenTransform::ScreenTransform;

    Transform GetScreenTransform() const override
    {
		auto pivot = GetPivot();

        Vector2 scaledSize = GetSize() * GetScale();
        Vector2 pivotOffset = scaledSize * pivot;
        Vector2 screenPos = GetPos() * GetSize() - pivotOffset;
        return { screenPos, GetSize(), GetScale(), pivot };
    }
};