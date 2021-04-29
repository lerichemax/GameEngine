#include "PCH.h"
#include "QubeObserver.h"

#include <thread>
#include <chrono>

#include "GameObject.h"
#include "QBertScene.h"
#include "Qube.h"
#include "QBert.h"

QubeObserver::QubeObserver(Pyramid* const pPyramid, QBert* const pQBert)
	: Observer(),
	m_pPyramid(pPyramid),
	m_pQBert(pQBert)
{
}

void QubeObserver::Notify(empire::Component* object, int event)
{
	switch ((QubeEvents)event)
	{
	case QubeEvents::PlayerJump:
		{
			auto qube = static_cast<Qube*>(object);
			auto qbertScene = static_cast<QBertScene*>(object->GetGameObject()->GetParentScene());
			qube->CharacterJumpIn(m_pQBert);
			switch (qbertScene->GetLevel())
			{
			case Level::Level1:
				qube->Flip();
				if (m_pPyramid->AreAllQubesFlipped())
				{
					std::cout << "YOU FINISHED LEVEL 1!\n";
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					qbertScene->ResetScene(Level::Level2);
				}
				break;
			case Level::Level2:
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
						std::cout << "YOU FINISHED LEVEL 2!\n";
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
						qbertScene->ResetScene(Level::Level3);
					}
				}
				break;
			case Level::Level3:
				if (qube->m_bIsFlipped)
				{
					qube->UnFlip();
					return;
				}
				else
				{
					qube->Flip();
					std::cout << "Qube flipped !\n";
					if (m_pPyramid->AreAllQubesFlipped())
					{
						std::cout << "YOU FINISHED LEVEL 3!\n";
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
						qbertScene->ResetScene(Level::Level1);
						//empire::SceneManager::GetInstance().ReloadCurrentScene();
					}
				}
				break;
			}
		}
		break;
	case QubeEvents::QubeFlipped:
		m_pQBert->EarnPoints(25);
		break;
	case QubeEvents::DiskUsed:
		m_pPyramid->DiskUsed();
		break;
	}
}