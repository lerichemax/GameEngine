#include "PCH.h"
#include "VersusGameManager.h"

#include "CharacterPoint.h"
#include "GameObject.h"
#include "Coily.h"
#include "CoilyCharacterController.h"
#include "QBert.h"
#include "VersusScene.h"
#include "Pyramid.h"
#include "TextRendererComponent.h"

VersusGameManager::VersusGameManager(empire::TextRendererComponent* pRoundText, empire::TextRendererComponent* pText1,
	empire::TextRendererComponent* pText2, CharacterPoint* pP1, CharacterPoint* pP2, Pyramid* pPyramid, unsigned int nbrRounds)
	:m_pRoundText(pRoundText),
	m_pTextP1(pText1),
	m_pTextP2(pText2),
	m_NbrRounds(nbrRounds),
	m_CurrentRound(1),
	m_pP1(pP1),
	m_pP2(pP2),
	m_pPyramid(pPyramid)
{
	
}

void VersusGameManager::Notify(empire::GameObject* object, int event)
{
	switch (VersusGameEvent(event))
	{
	case VersusGameEvent::Player1Died:
		{
		UpdateRound(object);
		UpdateCharacterPoint(m_pP2);
		m_pTextP2->SetText("P2: " + std::to_string(m_pP2->GetGameObject()->GetComponent<CharacterPoint>()->GetPoints()));
		auto pPlayer = object->GetComponent<QBert>();

		pPlayer->SetCurrentQube(m_pPyramid->GetTop());
		pPlayer->GetGameObject()->GetComponent<empire::RendererComponent>()->ChangeLayer(empire::Layer::foreground);
		pPlayer->SetCanMove();
		}
		break;
	case VersusGameEvent::Player2Died:
		{
			UpdateRound(object);
			UpdateCharacterPoint(m_pP1);
			m_pTextP1->SetText("P1: " + std::to_string(m_pP1->GetGameObject()->GetComponent<CharacterPoint>()->GetPoints()));

			auto pPlayer = object->GetComponent<Coily>();
			pPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
			pPlayer->GetGameObject()->GetComponent<empire::RendererComponent>()->ChangeLayer(empire::Layer::foreground);
		}
		break;
	case VersusGameEvent::PyramidCompleted:
		UpdateRound(object);
		UpdateCharacterPoint(m_pP1);
		m_pTextP1->SetText("P1: " + std::to_string(m_pP1->GetGameObject()->GetComponent<CharacterPoint>()->GetPoints()));
		
		break;
	case VersusGameEvent::GameOver:
		m_CurrentRound = 1;
		static_cast<VersusScene*>(object->GetParentScene())->ResetGame();
		break;
	case VersusGameEvent::IncreasePoints: //cancels out the increase point following a cube flip
		object->GetComponent<CharacterPoint>()->AddPoints(-25);
		break;
	case VersusGameEvent::CoilyTransform:
		object->GetComponent<CoilyCharacterController>()->SetEnable(false);
		//auto pCoily = object->GetComponent<Coily>();
		
		//InputManager::GetInstance().AddCommand(SDLK_UP, new MoveCommand(ConnectionDirection::upRight, pCoily));
		//InputManager::GetInstance().AddCommand(SDLK_RIGHT, new MoveCommand(ConnectionDirection::downRight, pCoily));
		//InputManager::GetInstance().AddCommand(SDLK_DOWN, new MoveCommand(ConnectionDirection::downLeft, pCoily));
		//InputManager::GetInstance().AddCommand(SDLK_LEFT, new MoveCommand(ConnectionDirection::upLeft, pCoily));
		break;
	}
}

void VersusGameManager::UpdateRound(empire::GameObject* pObject)
{
	m_CurrentRound++;
	m_pRoundText->SetText("Round " + std::to_string(m_CurrentRound));
	if (m_CurrentRound > 3)
	{
		static_cast<VersusScene*>(pObject->GetParentScene())->ResetGame();
	}
}

void VersusGameManager::UpdateCharacterPoint(CharacterPoint* pCharacterPoints)
{
	pCharacterPoints->AddPoints(1);

}