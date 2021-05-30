#include "PCH.h"
#include "Subject.h"
#include "GameObject.h"

using namespace empire;

Subject::Subject(Subject const& other)
	:m_pObservers{},
	m_NbrObservers(other.m_NbrObservers)
{
	for (unsigned int i{}; i < MAX_OBSERVERS; ++i)
	{
		m_pObservers[i] = other.m_pObservers[i];
	}
}

void Subject::AddObserver(Observer* pObserver)
{
	if (m_NbrObservers < MAX_OBSERVERS)
	{
		m_pObservers[m_NbrObservers] = pObserver;
		m_NbrObservers++;
	}
}

void Subject::RemoveObserver(Observer* pObserver)
{
	int toRemoveIndex{};
	for (unsigned int i = 0; i < m_NbrObservers; i++)
	{
		if (m_pObservers[i] == pObserver)
		{
			delete pObserver;
			pObserver = nullptr;
			toRemoveIndex = i;
			break;
		}
	}
	ShiftArray(toRemoveIndex + 1);
}

void Subject::Notify(GameObject* object, int event)
{
	for (unsigned int i = 0; i < m_NbrObservers; i++)
	{
		if (m_pObservers[i] != nullptr)
		{
			m_pObservers[i]->Notify(object, event);
		}
	}
}

void Subject::ShiftArray(int startIndex)
{
	for (unsigned int i = startIndex; i < m_NbrObservers; i++)
	{
		m_pObservers[i - 1] = m_pObservers[i];
	}
	m_pObservers[m_NbrObservers - 1] = nullptr;
}