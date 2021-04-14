#include "PCH.h"
#include "ObserverManager.h"

using namespace empire;

ObserverManager::~ObserverManager()
{
	for (auto pObs : m_pObservers)
	{
		delete pObs.second;
	}
	m_pObservers.clear();
}

void ObserverManager::AddObserver(unsigned int id, Observer* pObserver)
{
	m_pObservers.insert(std::make_pair(id,pObserver));
}