#pragma once
#include "SceneObject.h"

namespace dae
{
	enum class Event
	{
		PlayerDied
	};
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const SceneObject& object, Event event) = 0;
	};
}


