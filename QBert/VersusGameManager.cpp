#include "PCH.h"
#include "VersusGameManager.h"

#include "CharacterPoint.h"
#include "GameObject.h"
#include "Coily.h"
#include "CoilyCharacterController.h"
#include "QBert.h"

#include "Pyramid.h"
#include "TextRendererComponent.h"
#include "Timer.h"

VersusGameManager::VersusGameManager(TextRendererComponent* pRoundText, TextRendererComponent* pText1,
                                     TextRendererComponent* pText2, CharacterPoint* pP1, CharacterPoint* pP2, Pyramid* pPyramid,
                                     GameObject* pGameOver, unsigned int nbrRounds)
	:m_pRoundText(pRoundText),
	m_pTextP1(pText1),
	m_pTextP2(pText2),
	m_pP1(pP1),
	m_pP2(pP2),
	m_pPyramid(pPyramid),
	m_pGameOver(pGameOver),
	m_NbrRounds(nbrRounds),
	m_CurrentRound(1)
{
	
}

void VersusGameManager::Notify(GameObject* object, int event)
{
	switch (static_cast<VersusGameEvent>(event))
	{
	case VersusGameEvent::Player1Died:
		{
			UpdateRound();
			UpdateCharacterPoint(m_pP2);
			m_pTextP2->SetText("P2: " + std::to_string(m_pP2->GetGameObject()->GetComponent<CharacterPoint>()->GetPoints()));
			auto pPlayer = object->GetComponent<QBert>();

			pPlayer->SetCurrentQube(m_pPyramid->GetTop());
			pPlayer->GetGameObject()->GetComponent<RendererComponent>()->ChangeLayer(Layer::foreground);
			pPlayer->SetCanMove();
		}
		break;
	case VersusGameEvent::Player2Died:
		{
			UpdateRound();
			UpdateCharacterPoint(m_pP1);
			m_pTextP1->SetText("P1: " + std::to_string(m_pP1->GetGameObject()->GetComponent<CharacterPoint>()->GetPoints()));

			auto pPlayer = object->GetComponent<Coily>();
			pPlayer->SetCurrentQube(m_pPyramid->GetQube(2));
			pPlayer->GetGameObject()->GetComponent<RendererComponent>()->ChangeLayer(Layer::foreground);
		}
		break;
	case VersusGameEvent::PyramidCompleted:
		UpdateRound();
		UpdateCharacterPoint(m_pP1);
		m_pTextP1->SetText("P1: " + std::to_string(m_pP1->GetGameObject()->GetComponent<CharacterPoint>()->GetPoints()));
		
		break;
	case VersusGameEvent::GameOver:
		m_CurrentRound = 1;
		m_pGameOver->GetComponentInChildren<TextRendererComponent>()->SetText("Player 2 wins");
		Timer::GetInstance().SetTimeScale(0);
		m_pGameOver->SetActive(true);
		
		break;
	case VersusGameEvent::IncreasePoints: //cancels out the increase point following a cube flip
		object->GetComponent<CharacterPoint>()->AddPoints(-25);
		break;
	case VersusGameEvent::CoilyTransform:
		object->GetComponent<CoilyCharacterController>()->SetEnable(false);
		break;
	}
}

void VersusGameManager::UpdateRound()
{
	m_CurrentRound++;
	m_pRoundText->SetText("Round " + std::to_string(m_CurrentRound));
}

void VersusGameManager::UpdateCharacterPoint(CharacterPoint* pCharacterPoints)
{
	pCharacterPoints->AddPoints(1);
	CheckVictory();
}

void VersusGameManager::CheckVictory() const
{
	if (m_pP1->GetPoints() == 3)
	{
		m_pGameOver->GetComponentInChildren<TextRendererComponent>()->SetText("Player 1 wins");
		Timer::GetInstance().SetTimeScale(0);
		m_pGameOver->SetActive(true);
	}

	if (m_pP2->GetPoints() == 3)
	{
		m_pGameOver->GetComponentInChildren<TextRendererComponent>()->SetText("Player 2 wins");
		Timer::GetInstance().SetTimeScale(0);
		m_pGameOver->SetActive(true);
	}
}