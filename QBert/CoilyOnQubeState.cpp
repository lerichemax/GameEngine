#include "PCH.h"
#include "CoilyOnQubeState.h"

#include "CoilyCharacterController.h"
#include "Coily.h"
#include "Qube.h"
#include "JumpingState.h"
#include "FallingState.h"

#include "Timer.h"


CoilyOnQubeState::CoilyOnQubeState(Character* pChar, Jumper* pJumper, CoilyCharacterController* pCharCont)
	:EnemyOnQubeState(pChar, pJumper, pCharCont),
	m_pCharController(pCharCont)
{
	
}

CharacterState* CoilyOnQubeState::Update(empire::GameObject* pObject)
{
	if (!m_pCharController->IsEnable())
	{
		return nullptr;
	}
	if (!m_pCharController->m_pCoilyCharacter->IsTransformed())
	{
		
		if (m_pCharController->m_pCoilyCharacter->GetCurrentQube()->IsLastRow())
		{
			m_pCharController->m_pCoilyCharacter->Transform(true);
			m_pCharController->FindQBert();

		}
		else
		{
			return EnemyOnQubeState::Update(pObject);
		}
	}
	
	if (!m_pCharController->m_bIsIdle)
	{
		if (m_pCharController->m_MoveTimer < m_pCharController->MOVE_MAX_TIME)
		{
			m_pCharController->m_MoveTimer += empire::Timer::GetInstance().GetDeltaTime();
			return nullptr;
		}

		if (m_pCharController->m_CurrentlyInQueue != 0)
		{
			auto direction = m_pCharController->ChooseDirection();
			auto qube = m_pCharacter->GetCurrentQube();
			
			m_pCharController->Move(direction);
			m_pCharController->m_MoveTimer = 0;
			

			if (!qube->HasConnection(direction))
			{
				return new FallingState(m_pCharacter, m_pJumperComp);
			}
			else
			{
				return new JumpingState(m_pCharacter, m_pJumperComp);
			}
		}
	}
	return nullptr;
}