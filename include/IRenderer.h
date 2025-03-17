#pragma once

#include "memory"
#include "ITexture.h"

class IRenderer {
public:
	virtual ~IRenderer() = default;
	virtual void Clear() const = 0;
	virtual void DrawRect(float x, float y, float w, float h) const = 0;
	virtual void Present() const = 0;
	virtual void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const = 0;
	virtual std::shared_ptr<ITexture> CreateTexture(int width, int height, const unsigned char* data) const = 0;
	virtual void DrawTexture(float x, float y, float w, float h, const ITexture &texture) const = 0;
};