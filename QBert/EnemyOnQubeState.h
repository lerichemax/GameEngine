#pragma once
#include "OnQubeState.h"

class EnemyCharacterController;

class EnemyOnQubeState : public OnQubeState
{
public:
	explicit EnemyOnQubeState(Characters* pChar, Jumper* pJumper, EnemyCharacterController* m_pCharCont);
	CharacterState* Update(GameObject* pObject) override;

private:
	EnemyCharacterController* m_pCharController;
};
