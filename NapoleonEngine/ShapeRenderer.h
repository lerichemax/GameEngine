#pragma once
#include "System.h"

class ShapeRenderer final : public System
{
public:
	void Update(ComponentManager* const pComponentManager) override;

	void SetSignature(Coordinator* const pRegistry) override;
};