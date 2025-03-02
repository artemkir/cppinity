#pragma once

class IRenderer {
public:
	virtual ~IRenderer() = default;
	virtual void Clear(int r, int g, int b) = 0;
	virtual void DrawRect(int x, int y, int w, int h) = 0;
	virtual void Present() = 0;
	virtual void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;
};