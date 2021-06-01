#include "PCH.h"
#include "QubeObserver.h"

#include <thread>
#include <chrono>

#include "GameObject.h"
#include "QBertScene.h"
#include "Qube.h"
#include "QBert.h"

QubeObserver::QubeObserver(Pyramid* const pPyramid)
	: Observer(),
	m_pPyramid(pPyramid)
{
}

void QubeObserver::Notify(empire::GameObject* object, int event)
{
	switch ((QubeEvents)event)
	{
	case QubeEvents::PlayerJump:
		{
		auto qube = object->GetComponent<Qube>();
			auto qbertScene = static_cast<QBertScene*>(object->GetParentScene());
			switch (qbertScene->GetLevel())
			{
			case QBertScene::Level::Level1:
				qube->Flip();
				if (m_pPyramid->AreAllQubesFlipped())
				{
					Debugger::GetInstance().Log("YOU FINISHED LEVEL 1!");
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					qbertScene->ResetScene(QBertScene::Level::Level2);
				}
				break;
			case QBertScene::Level::Level2:
				if (qube->m_JumpCounter == 0)
				{
					qube->IntermediateFlip();
					return;
				}
				else if (qube->m_JumpCounter == 1)
				{
					qube->Flip();
					if (m_pPyramid->AreAllQubesFlipped())
					{
						Debugger::GetInstance().Log("YOU FINISHED LEVEL 2!");
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
						qbertScene->ResetScene(QBertScene::Level::Level3);
					}
				}
				break;
			case QBertScene::Level::Level3:
				if (qube->m_bIsFlipped)
				{
					qube->UnFlip();
					return;
				}
				else
				{
					qube->Flip();
					Debugger::GetInstance().Log("Qube flipped !");
					if (m_pPyramid->AreAllQubesFlipped())
					{
						Debugger::GetInstance().Log("YOU FINISHED LEVEL 3!");
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
						qbertScene->EndGame(true);
					}
				}
				break;
			}
		}
		break;
	case QubeEvents::QubeFlipped:
		static_cast<QBert*>(object->GetComponent<Qube>()->GetCharacter())->EarnPoints(25);
		break;
	case QubeEvents::DiskUsed:
		m_pPyramid->DiskUsed();
		break;
	}
}