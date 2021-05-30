#pragma once


namespace empire
{
	
	class GameObject;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(GameObject* object, int event) = 0;
	};
}


