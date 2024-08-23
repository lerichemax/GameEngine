#pragma once
#include "Component.h"

#include "System.h"

struct FPSCounter : public Component
{
	int m_FPS;

	void Serialize(StreamWriter& writer) const override;
};