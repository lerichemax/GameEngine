#pragma once
#include "Component.h"
#include "Observer.h"

constexpr unsigned int MAX_OBSERVERS = 32;

namespace empire
{
	class Subject
	{
	public:
		Subject() = default;
		Subject(Subject const& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(Subject const& rhs) = delete;
		Subject& operator=(Subject&& rhs) = delete;
		~Subject();

		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void Notify(Component* object, Event event);
	
	private:
		Observer* m_pObservers[MAX_OBSERVERS];
		unsigned int m_NbrObservers;

		void ShiftArray(int startIndex);
	};
}