#pragma once

class IRendererComponent : public BaseComponent {
public:
	virtual void Render(IRenderer& renderer) = 0;
	virtual int Order() = 0;
};