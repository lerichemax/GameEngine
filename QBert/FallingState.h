#pragma once
#include "CharacterState.h"

class FallingState final : public CharacterState
{
public:
	explicit FallingState(Characters* pChar, Jumper* pJumper);
	~FallingState() = default;

	CharacterState* Update(GameObject* pObject);
};
