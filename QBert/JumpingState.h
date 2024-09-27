#pragma once
#include "CharacterState.h"

class JumpingState final : public CharacterState
{
public:
	explicit JumpingState(Characters* pChar, Jumper* pJumper);
	~JumpingState() = default;

	void Enter() override;

	CharacterState* Update(GameObject* pObject) ;
};