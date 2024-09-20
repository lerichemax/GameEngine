#pragma once
#include "System.h"

class TextRendererSystem : public System
{
public:
	void Update() override;

	void SetSignature() const override;
};