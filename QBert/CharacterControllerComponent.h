#pragma once
#include "Component.h"

struct CharacterControllerComponent : public Component
{
	void Serialize(StreamWriter& writer) const override;

	bool m_BlockInput = false;

	SDL_Scancode Right;
	SDL_Scancode Left;
	SDL_Scancode Up;
	SDL_Scancode Down;
};
