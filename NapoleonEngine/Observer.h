#pragma once


namespace empire
{
	enum class Event
	{
		PlayerDied,
		IncreasePoints
	};
	class Component;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Component* object, Event event) = 0;
	};
}


