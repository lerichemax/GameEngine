#pragma once
#include "Component.h"

struct CharacterControllerComponent : public ecs::Component
{
	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	bool m_BlockInput = false;

	int PlayerNumber;

	SDL_Scancode Right;
	SDL_Scancode Left;
	SDL_Scancode Up;
	SDL_Scancode Down;
};
