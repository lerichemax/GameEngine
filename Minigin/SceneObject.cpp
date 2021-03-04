#include "MiniginPCH.h"
#include "SceneObject.h"

using namespace dae;

SceneObject::SceneObject(Tag tag)
	:m_pComponents{},
	m_IsActive{true},
	m_Tag{tag}
{
	
}

SceneObject::~SceneObject()
{
	for (auto comp : m_pComponents)
	{
		delete comp;
	}
}

void SceneObject::AddObserver(Observer* pObserver)
{
	if (m_NbrObservers < MAX_OBSERVERS)
	{
		m_pObservers[m_NbrObservers] = pObserver;
		m_NbrObservers++;
	}
}

void SceneObject::RemoveObserver(Observer* pObserver)
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

void SceneObject::Notify(SceneObject* object, Event event)
{
	for (unsigned int i = 0; i < m_NbrObservers; i++)
	{
		m_pObservers[i]->Notify(object, event);
	}
}

void SceneObject::ShiftArray(int startIndex)
{
	for (unsigned int i = startIndex; i < m_NbrObservers; i++)
	{
		m_pObservers[i - 1] = m_pObservers[i];
	}
	m_pObservers[m_NbrObservers - 1] = nullptr;
}