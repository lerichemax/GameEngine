#include "PCH.h"
#include "FallingState.h"

#include "Jumper.h"
#include "OnQubeState.h"
#include "Character.h"
#include "CoilyCharacterController.h"
#include "CoilyOnQubeState.h"

#include "GameObject.h"

FallingState::FallingState(Character* pChar, Jumper* pJumper)
	:CharacterState(pChar, pJumper, CharacterStateType::falling)
{
	
}

CharacterState* FallingState::Update(GameObject* pObject)
{
	//m_pJumperComp->UpdateFall(pObject->GetECSTransform());
	//if (m_pJumperComp->IsDead())
	//{
	//	m_pJumperComp->SetIsNotDead();
	//	
	//	m_pCharacter->DoDie();
	//	
	//	switch (m_pCharacter->m_Type)
	//	{
	//	case CharacterType::player:
	//		return new OnQubeState(m_pCharacter, m_pJumperComp);
	//	case CharacterType::coily:
	//		if (pObject->HasComponent<CoilyCharacterController>())
	//		{
	//			/*return new CoilyOnQubeState(m_pCharacter, m_pJumperComp,
	//				pObject->GetComponent<CoilyCharacterController>());*/
	//		}
	//		else
	//		{
	//			return new OnQubeState(m_pCharacter, m_pJumperComp);
	//		}
	//	default:
	//		return nullptr;
	//	}
	//}
	return nullptr;
}