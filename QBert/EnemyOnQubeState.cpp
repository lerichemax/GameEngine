#include "PCH.h"
#include "EnemyOnQubeState.h"

#include "EnemyCharacterController.h"
#include "Enemy.h"
#include "JumpingState.h"
#include "FallingState.h"
#include "Qube.h"

#include "Timer.h"
#include "WrongWay.h"

EnemyOnQubeState::EnemyOnQubeState(Character* pChar, Jumper* pJumper, EnemyCharacterController* pCharCont)
	:OnQubeState(pChar, pJumper),
	m_pCharController(pCharCont)
{
	
}

CharacterState* EnemyOnQubeState::Update(empire::GameObject* )
{
	if (m_pCharController->m_MoveTimer < m_pCharController->MOVE_MAX_TIME)
	{
		m_pCharController->m_MoveTimer += empire::Timer::GetInstance().GetDeltaTime();
		return nullptr;
	}

	m_pCharController->m_MoveTimer = 0;
	auto direction = m_pCharController->ChooseDirection();
	auto qube = m_pCharacter->GetCurrentQube();
	m_pCharController->m_pEnemyCharacter->Move(direction);

	if ((m_pCharacter->GetType() != CharacterType::wrongWay && !qube->HasConnection(direction)) ||
		(m_pCharacter->GetType() == CharacterType::wrongWay && 
		!qube->HasEscheresqueConnection(direction, static_cast<WrongWay*>(m_pCharacter)->IsEscheresqueRight())))
	{
		return new FallingState(m_pCharacter, m_pJumperComp);
	}
	
	return new JumpingState(m_pCharacter, m_pJumperComp);

}