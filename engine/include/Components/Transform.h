#pragma once

#include "Std.h"
#include "BaseComponent.h"

class Canvas;

struct Vector2
{
    float x = 0.0f;
    float y = 0.0f;

    Vector2(float x_, float y_) : x(x_), y(y_) {}
    Vector2(int x_, int y_) {
        x = static_cast<float>(x_);
        y = static_cast<float>(y_);
    }

    friend Vector2 operator+(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x + v2.x, v1.y + v2.y }; }
    friend Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x - v2.x, v1.y - v2.y }; }
    friend Vector2 operator*(const Vector2& v1, const Vector2& v2) { return Vector2{ v1.x * v2.x, v1.y * v2.y }; }
    friend Vector2 operator/(const Vector2& v1, const Vector2& v2)
    {
        assert(v2.x != 0.0f && v2.y != 0.0f && "Division by zero");
        return Vector2{ v1.x / v2.x, v1.y / v2.y };
    }

    friend Vector2 operator*(const Vector2& v, float scalar) { return Vector2{ v.x * scalar, v.y * scalar }; }
    friend Vector2 operator/(const Vector2& v, float scalar)
    {
        assert(scalar != 0.0f && "Division by zero");
        return Vector2{ v.x / scalar, v.y / scalar };
    }

    List<float> to_vector() const { return { x, y }; }
};

class BaseTransform : public BaseComponent
{
public:
    struct Transform
    {
        Vector2 pos;
        Vector2 size;
        Vector2 scale;
        Vector2 pivot; // New: Pivot point (0 to 1, e.g., 0.5,0.5 = center)
    };

    virtual ~BaseTransform() = default;
    virtual Transform GetScreenTransform() const = 0;
    virtual Transform GetFinalScreenTransform() const = 0;
    virtual float GetX() const = 0;
    virtual float GetY() const = 0;
    virtual float GetScaleX() const = 0;
    virtual float GetScaleY() const = 0;
    virtual float GetWidth() const = 0;
    virtual float GetHeight() const = 0;
    virtual const Vector2& GetPivot() const = 0; // New
    virtual void SetSize(float width_, float height_) = 0;
    virtual void SetPosition(float x_, float y_) = 0;
    virtual void SetScale(float x_, float y_) = 0;
    virtual void SetPivot(float x_, float y_) = 0; // New
};

class ScreenTransform : public BaseTransform
{
    Transform t{ {0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f} }; // Default pivot to top-left
    ScreenTransform* parent = nullptr;
    Canvas* canvas = nullptr;

    Canvas* GetAncestorCanvas() const;
    Transform GetRelativeTransformToAncestor(const ScreenTransform* ancestor) const;

public:
    explicit ScreenTransform(Vector2 pos, Vector2 size, Vector2 scale = Vector2{ 1.0f, 1.0f }, Vector2 pivot = Vector2{ 0.0f, 0.0f })
        : t{ pos, size, scale, pivot } {
    }

    const Vector2& GetPos() const { return t.pos; }
    const Vector2& GetSize() const { return t.size; }
    const Vector2& GetScale() const { return t.scale; }
    const Vector2& GetPivot() const override { return t.pivot; } // New
    float GetX() const override { return GetPos().x; }
    float GetY() const override { return GetPos().y; }
    float GetScaleX() const override { return GetScale().x; }
    float GetScaleY() const override { return GetScale().y; }
    float GetWidth() const override { return GetSize().x; }
    float GetHeight() const override { return GetSize().y; }
    void SetSize(float width_, float height_) override { t.size = { width_, height_ }; }
    void SetPosition(float x_, float y_) override { t.pos = { x_, y_ }; }
    void SetScale(float x_, float y_) override { t.scale = { x_, y_ }; }
    void SetPivot(float x_, float y_) override { t.pivot = { x_, y_ }; } // New

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
        Vector2 scaledSize = GetSize() * GetScale();
        Vector2 pivotOffset = scaledSize * GetPivot();
        Vector2 screenPos = GetPos() * GetSize() - pivotOffset;
        return { screenPos, GetSize(), GetScale(), GetPivot() }; // Preserve size and scale
    }

    Transform GetFinalScreenTransform() const override;
};