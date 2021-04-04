#include "PCH.h"
#include "GameObserver.h"

#include <thread>
#include <chrono>

#include "SceneManager.h"

GameObserver::GameObserver(GameController* const pController)
	: Observer(),
	m_pController(pController)
{
	
}

void GameObserver::Notify(empire::Component* object, int event)
{
	switch ((GameEvents)event)
	{
	case GameEvents::QubeFlipped:
		std::cout << "Qube flipped !\n";
		if (m_pController->AreAllQubesFlipped())
		{
			std::cout << "YOU WON !\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			empire::SceneManager::GetInstance().ReloadCurrentScene();
		}
		break;
	}
}