#include "PCH.h"
#include  "JumpingState.h"

#include "Jumper.h"
#include "OnQubeState.h"
#include "Character.h"
#include "CoilyCharacterController.h"
#include "CoilyOnQubeState.h"

#include "GameObject.h"

JumpingState::JumpingState(Character* pChar, Jumper* pJumper)
	:CharacterState(pChar, pJumper, CharacterStateType::jumping)
{
	
}

void JumpingState::Enter()
{
	m_pCharacter->SwitchToJumpTex();
}



CharacterState* JumpingState::Update(GameObject* pObject)
{
	m_pJumperComp->UpdateJump(pObject->GetTransform());
	if (!m_pJumperComp->IsJumping())
	{
		m_pCharacter->LandOnQube();
		m_pCharacter->MoveToCurrentQube();
		switch (m_pCharacter->m_Type)
		{
		case CharacterType::player:
			return new OnQubeState(m_pCharacter, m_pJumperComp);
			break;
		case CharacterType::coily:
			if (pObject->HasComponent<CoilyCharacterController>())
			{
				return new CoilyOnQubeState(m_pCharacter, m_pJumperComp,
					pObject->GetComponent<CoilyCharacterController>());
			}
			else
			{
				return new OnQubeState(m_pCharacter, m_pJumperComp);
			}
			break;
		default:
			return new EnemyOnQubeState(m_pCharacter, m_pJumperComp,
				pObject->GetComponent<EnemyCharacterController>());
			break;
		}
	}
	return nullptr;
}