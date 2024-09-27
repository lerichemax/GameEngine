#pragma once
#include "EnemyOnQubeState.h"

class CoilyCharacterController;

class CoilyOnQubeState final : public EnemyOnQubeState
{
public:
	explicit CoilyOnQubeState(Characters* pChar, Jumper* pJumper, CoilyCharacterController* pCharCont);
	CharacterState* Update(GameObject* pObject);

private:
	CoilyCharacterController* m_pCharController;
};
