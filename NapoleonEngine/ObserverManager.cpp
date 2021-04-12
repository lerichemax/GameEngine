#include "PCH.h"
#include "ObserverManager.h"

using namespace empire;

ObserverManager::~ObserverManager()
{
	for (auto pObs : m_pObservers)
	{
		delete pObs;
	}
	m_pObservers.clear();
}

void ObserverManager::AddObserver(Observer* pObserver)
{
	m_pObservers.push_back(pObserver);
}