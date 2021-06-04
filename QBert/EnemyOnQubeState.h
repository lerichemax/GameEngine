#pragma once
#include "OnQubeState.h"

class EnemyCharacterController;

class EnemyOnQubeState : public OnQubeState
{
public:
	EnemyOnQubeState(Character* pChar, Jumper* pJumper, EnemyCharacterController* m_pCharCont);
	CharacterState* Update(empire::GameObject* pObject) override;

private:
	EnemyCharacterController* m_pCharController;
};
