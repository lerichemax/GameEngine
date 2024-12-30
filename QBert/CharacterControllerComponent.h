#pragma once
#include "Component.h"

struct CharacterControllerComponent : public ecs::Component
{
	bool m_BlockInput = false;

	PROPERTY(int, PlayerNumber);

	PROPERTY(SDL_Scancode, Right);
	PROPERTY(SDL_Scancode, Left);
	PROPERTY(SDL_Scancode, Up);
	PROPERTY(SDL_Scancode, Down);
};

SERIALIZE_CLASS(CharacterControllerComponent, ecs::Component);