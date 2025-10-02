#pragma once

#include "memory"
#include "ITexture.h"

class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual void BeginPass() const = 0;
	virtual void DrawRect(float x, float y, float w, float h, float col[4]) const = 0;
	virtual void EndPass() const = 0;
	virtual std::shared_ptr<ITexture> CreateTexture(int width, int height, const unsigned char *data) const = 0;
	virtual void DrawTexture(float x, float y, float w, float h, const ITexture &texture) const = 0;
};