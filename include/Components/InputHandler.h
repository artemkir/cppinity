#pragma once

#include "IInputHandler.h"

class InputHandler : public IInputHandler {
	Direction dir;
	
public:
	InputHandler();
	void Update(float deltaTime) override;
	Direction GetDirection() override;
};