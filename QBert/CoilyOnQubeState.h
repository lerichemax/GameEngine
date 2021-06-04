#pragma once
#include "EnemyOnQubeState.h"

class CoilyCharacterController;

class CoilyOnQubeState final : public EnemyOnQubeState
{
public:
	CoilyOnQubeState(Character* pChar, Jumper* pJumper, CoilyCharacterController* pCharCont);
	CharacterState* Update(GameObject* pObject) override;

private:
	CoilyCharacterController* m_pCharController;
};
