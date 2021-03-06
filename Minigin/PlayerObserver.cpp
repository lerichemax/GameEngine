#include "MiniginPCH.h"
#include "PlayerObserver.h"
#include "QBert.h"
#include "TextRendererComponent.h"
#include "UIObject.h"
#include <iostream>

using namespace dae;

PlayerObserver::PlayerObserver(dae::UIObject* const pPoints, dae::UIObject* const pLives)
	:m_pPointsCounter(pPoints),
	m_LivesCounter(pLives)
{
	
}

void PlayerObserver::Notify(Component* object, Event event)
{
	switch (event)
	{
	case Event::PlayerDied:
		m_LivesCounter->GetComponent<TextRendererComponent>().SetText("P"+ 
			std::to_string(static_cast<QBert*>(object)->GetPlayerNumber()) + " Lives: " +
			std::to_string(static_cast<QBert*>(object)->GetLives()));
		
		std::cout << "Player died !\n";
		break;
	case Event::IncreasePoints:
		{
		m_pPointsCounter->GetComponent<TextRendererComponent>().SetText("P" + 
			std::to_string(static_cast<QBert*>(object)->GetPlayerNumber()) + " Points: " +
			std::to_string(static_cast<QBert*>(object)->GetPoints()));
		}
	}
}