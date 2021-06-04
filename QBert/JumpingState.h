#pragma once
#include "CharacterState.h"

class JumpingState final : public CharacterState
{
public:
	JumpingState(Character* pChar, Jumper* pJumper);
	~JumpingState() = default;

	void Enter() override;

	CharacterState* Update(GameObject* pObject) override;
};