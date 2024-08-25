#pragma once
#include "Component.h"

struct CharacterComponent : public Component
{
	Entity CurrentQube;

	void Serialize(StreamWriter& writer) const override;
};