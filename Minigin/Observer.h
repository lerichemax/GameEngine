#pragma once


namespace dae
{
	enum class Event
	{
		PlayerDied,
		IncreasePoints
	};
	class SceneObject;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(SceneObject* object, Event event) = 0;
	};
}


