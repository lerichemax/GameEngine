#pragma once
#include "System.h"

class ShapeRenderer final : public System
{
public:
	void Update() override;

	void SetSignature(Coordinator* const pRegistry) override;
};