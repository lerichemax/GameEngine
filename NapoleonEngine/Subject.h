#pragma once
#include "Component.h"
#include "Observer.h"

constexpr unsigned int MAX_OBSERVERS = 32;

class Subject
{
public:
	Subject() = default;
	Subject(Subject const& other);
	Subject(Subject&& other) = delete;
	Subject& operator=(Subject const& rhs) = delete;
	Subject& operator=(Subject&& rhs) = delete;
	~Subject() = default;

	void AddObserver(Observer* pObserver);
	void RemoveObserver(Observer* pObserver);
	void Notify(GameObject* object, int event);
	
private:
	Observer* m_pObservers[MAX_OBSERVERS];
	unsigned int m_NbrObservers;

	void ShiftArray(int startIndex);
};