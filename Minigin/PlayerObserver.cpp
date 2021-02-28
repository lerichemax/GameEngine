#include "MiniginPCH.h"
#include "PlayerObserver.h"

#include <iostream>
using namespace dae;

void PlayerObserver::Notify(const SceneObject& object, Event event)
{
	switch (event)
	{
	case Event::PlayerDied:
		if (object.HasTag(Tag::player))
		{
			std::cout << "Player died !\n";
		}
		break;
	}
}