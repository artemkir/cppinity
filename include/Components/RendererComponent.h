#pragma once

#include "BaseComponent.h"

class IRenderer;

class RendererComponent : public BaseComponent
{
	int renderOrder;
	IRenderer *renderer = nullptr;

protected:
	IRenderer *GetRenderer() const { return renderer; }

public:
	explicit RendererComponent(int renderOrder_ = 0)
		: renderOrder(renderOrder_) {}

	~RendererComponent() override = default;
	virtual void Render() = 0;
	int GetRenderOrder() const { return renderOrder; };
	void SetRenderer(IRenderer *renderer_) { renderer = renderer_; }
};