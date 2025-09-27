#include "Components/Transform.h"

TileTransform::TileTransform(float x_, float y_, float w, float h)
    : x(x_), y(y_), width(w), height(h), scaleX(1.0f), scaleY(1.0f) {}

float TileTransform::GetX() const { return x; }
float TileTransform::GetY() const { return y; }
float TileTransform::GetScaleX() const { return scaleX; }
float TileTransform::GetScaleY() const { return scaleY; }

void TileTransform::SetSize(float width_, float height_) { 
    width = width_; 
    height = height_; 
}

void TileTransform::SetPosition(float x_, float y_) { 
    x = x_; 
    y = y_; 
}

void TileTransform::SetScale(float x_, float y_) { 
    scaleX = x_; 
    scaleY = y_; 
}

float TileTransform::GetWidth() const { return width; }
float TileTransform::GetHeight() const { return height; }

BaseTransform::Transform TileTransform::GetScreenTransform() { 
    return { 
        x * width - (scaleX - 1.0f) * width / 2.0f,
        y * height - (scaleY - 1.0f) * height / 2.0f,
        width * scaleX, 
        height * scaleY 
    }; 
}