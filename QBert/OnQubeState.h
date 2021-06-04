#pragma once
#include "CharacterState.h"

class OnQubeState : public CharacterState
{
public:
	OnQubeState(Character* pChar, Jumper* pJumper);
	~OnQubeState() = default;

	void Enter() override;
};
