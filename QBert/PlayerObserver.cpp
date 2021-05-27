#include "PCH.h"
#include "PlayerObserver.h"

#include "EnemyManager.h"
#include "QBert.h"
#include "QBertScene.h"
#include "Pyramid.h"
#include "EnemyManager.h"

#include "TextRendererComponent.h"

using namespace empire;

PlayerObserver::PlayerObserver(TextRendererComponent* const pPoints, TextRendererComponent* const pLives, 
	Pyramid* const pPyramid, EnemyManager* const pManager)
	:m_pPointsCounter(pPoints),
	m_LivesCounter(pLives),
	m_pPyramid(pPyramid),
	m_pEnemyManager(pManager)
{
}

void PlayerObserver::Notify(Component* object, int event)
{
	switch ((PlayerEvent)event)
	{
	case PlayerEvent::PlayerDied:
		{
		auto pQ = static_cast<QBert*>(object);
			m_LivesCounter->SetText("P" +
				std::to_string(pQ->GetPlayerNumber()) + " Lives: " +
				std::to_string(pQ->GetLives()));
			m_pEnemyManager->Reset();
			
			pQ->SetCurrentQube(pQ->GetCurrentQube());
			pQ->GetGameObject()->GetComponent<RendererComponent>()->ChangeLayer(Layer::foreground);
			pQ->SetCanMove();
			
			Debugger::GetInstance().Log("YOU DIED !");
			break;
		}
	case PlayerEvent::PlayerJumpOut:
		m_LivesCounter->SetText("P" +
			std::to_string(static_cast<QBert*>(object)->GetPlayerNumber()) + " Lives: " +
			std::to_string(static_cast<QBert*>(object)->GetLives()));
		object->GetGameObject()->GetComponent<QBert>()->SetCurrentQube(m_pPyramid->GetTop());
		m_pPyramid->Reset();
		m_pEnemyManager->Reset();
		Debugger::GetInstance().Log("YOU DIED !");
		break;
	case PlayerEvent::IncreasePoints:
		{
		m_pPointsCounter->SetText("P" + 
			std::to_string(static_cast<QBert*>(object)->GetPlayerNumber()) + " Points: " +
			std::to_string(static_cast<QBert*>(object)->GetPoints()));
		}
		break;
	case PlayerEvent::GameOver:
		Debugger::GetInstance().Log("GAME OVER");
		static_cast<QBertScene*>(object->GetGameObject()->GetParentScene())->ResetGame();
		break;
	case PlayerEvent::JumpOnDisk:
		m_pEnemyManager->SetCoiliesIdle(true);
		break;
	case PlayerEvent::JumpOffDisk:
		m_pEnemyManager->SetCoiliesIdle(false);
		break;
	}
}