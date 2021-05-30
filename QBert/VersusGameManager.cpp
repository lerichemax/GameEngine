#include "PCH.h"
#include "VersusGameManager.h"

#include "GameObject.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "Qube.h"
#include "Coily.h"
#include "CoilyCharacterController.h"
#include "QBert.h"
#include "VersusScene.h"

#include "TextRendererComponent.h"

VersusGameManager::VersusGameManager(empire::TextRendererComponent* pRoundText, empire::TextRendererComponent* pText1,
	empire::TextRendererComponent* pText2, unsigned int nbrRounds)
	:m_pRoundText(pRoundText),
	m_pTextP1(pText1),
	m_pTextP2(pText2),
	m_NbrRounds(nbrRounds),
	m_CurrentRound()
{
	
}

void VersusGameManager::Notify(empire::GameObject* object, int event)
{
	switch (VersusGameEvent(event))
	{
	case VersusGameEvent::Player1Died:
		break;
	case VersusGameEvent::Player2Died:
		break;
	case VersusGameEvent::PyramidCompleted:
		break;
	case VersusGameEvent::GameOver:
		static_cast<VersusScene*>(object->GetParentScene());
		break;
	case VersusGameEvent::CoilyTransform:
		object->GetComponent<CoilyCharacterController>()->SetEnable(false);
		auto pCoily = object->GetComponent<Coily>();
		
		InputManager::GetInstance().AddCommand(SDLK_UP, new MoveCommand(ConnectionDirection::upRight, pCoily));
		InputManager::GetInstance().AddCommand(SDLK_RIGHT, new MoveCommand(ConnectionDirection::downRight, pCoily));
		InputManager::GetInstance().AddCommand(SDLK_DOWN, new MoveCommand(ConnectionDirection::downLeft, pCoily));
		InputManager::GetInstance().AddCommand(SDLK_LEFT, new MoveCommand(ConnectionDirection::upLeft, pCoily));
		break;
	}
}