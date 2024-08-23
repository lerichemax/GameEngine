#pragma once
#include "Component.h"

struct CharacterControllerComponent : public Component
{
	void Serialize(StreamWriter& writer) const override;

	bool m_BlockInput = false;
};
