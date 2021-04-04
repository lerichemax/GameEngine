#pragma once


namespace empire
{
	
	class Component;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Component* object, int event) = 0;
	};
}


