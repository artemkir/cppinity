// Vector2.h
#pragma once

#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector2
{
    float x = 0.0f;
    float y = 0.0f;

    Vector2() = default;
    Vector2(const Vector2& other) = default;
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


#endif // VECTOR2_H