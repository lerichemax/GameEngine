#include "PCH.h"
#include "PlayerObserver.h"

#include "Coily.h"
#include "EnemyManager.h"
#include "TextRendererComponent.h"
#include "UIObject.h"

#include "QBert.h"
#include "QBertScene.h"
#include "Pyramid.h"
#include "Qube.h"

using namespace empire;

PlayerObserver::PlayerObserver(empire::UIObject* const pPoints, empire::UIObject* const pLives, Pyramid* const pPyramid)
	:m_pPointsCounter(pPoints),
	m_LivesCounter(pLives),
	m_pPyramid(pPyramid)
{
}

void PlayerObserver::Notify(Component* object, int event)
{
	switch ((PlayerEvent)event)
	{
	case PlayerEvent::PlayerDied:
		{
		auto pQ = static_cast<QBert*>(object);
			m_LivesCounter->GetComponent<TextRendererComponent>()->SetText("P" +
				std::to_string(pQ->GetPlayerNumber()) + " Lives: " +
				std::to_string(pQ->GetLives()));
			m_pPyramid->GetEnemyManager()->Reset();
			
			pQ->SetCurrentQube(pQ->GetCurrentQube());
			pQ->GetGameObject()->GetComponent<RendererComponent>()->ChangeLayer(Layer::foreground);
			pQ->SetCanMove();
			
			std::cout << "YOU DIED !\n";
			break;
		}
	case PlayerEvent::PlayerJumpOut:
		m_LivesCounter->GetComponent<TextRendererComponent>()->SetText("P" +
			std::to_string(static_cast<QBert*>(object)->GetPlayerNumber()) + " Lives: " +
			std::to_string(static_cast<QBert*>(object)->GetLives()));
		object->GetGameObject()->GetComponent<QBert>()->SetCurrentQube(m_pPyramid->GetTop());
		m_pPyramid->Reset();
		std::cout << "YOU DIED !\n";
		break;
	case PlayerEvent::IncreasePoints:
		{
		m_pPointsCounter->GetComponent<TextRendererComponent>()->SetText("P" + 
			std::to_string(static_cast<QBert*>(object)->GetPlayerNumber()) + " Points: " +
			std::to_string(static_cast<QBert*>(object)->GetPoints()));
		}
		break;
	case PlayerEvent::GameOver:
		std::cout << "GAME OVER\n";
		static_cast<QBertScene*>(object->GetGameObject()->GetParentScene())->ResetGame();
		break;
	case PlayerEvent::JumpOnDisk:
		m_pPyramid->GetEnemyManager()->SetCoiliesIdle(true);
		break;
	case PlayerEvent::JumpOffDisk:
		m_pPyramid->GetEnemyManager()->SetCoiliesIdle(false);
		break;
	}
}