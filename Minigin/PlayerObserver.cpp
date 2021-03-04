#include "MiniginPCH.h"
#include "PlayerObserver.h"
#include "QBert.h"
#include "TextRendererComponent.h"
#include <iostream>
using namespace dae;

PlayerObserver::PlayerObserver(dae::UIObject* pPoints)
	:m_pPointsCounter(pPoints)
{
	
}

void PlayerObserver::Notify(SceneObject* object, Event event)
{
	switch (event)
	{
	case Event::PlayerDied:
		if (object->HasTag(Tag::player1) || object->HasTag(Tag::player2))
		{
			std::cout << "Player died !\n";
		}
		break;
	case Event::IncreasePoints:
		{
			if (object->HasTag(Tag::player1) || object->HasTag(Tag::player2))
			{
				m_pPointsCounter->GetComponent<TextRendererComponent>().SetText("Points: " +
					std::to_string(static_cast<QBert*>(object)->GetPoints()));
			}
		}
	}
}