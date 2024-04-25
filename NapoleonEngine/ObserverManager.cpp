#include "PCH.h"
#include "ObserverManager.h"

class ObserverManager::ObserverManagerImpl
{
public:
	ObserverManagerImpl();
	ObserverManagerImpl(ObserverManagerImpl const& other) = delete;
	ObserverManagerImpl(ObserverManagerImpl&& other) = delete;
	ObserverManagerImpl& operator=(ObserverManagerImpl const& rhs) = delete;
	ObserverManagerImpl& operator=(ObserverManagerImpl&& rhs) = delete;
	~ObserverManagerImpl();

	void AddObserver(Observer* pObserver);
private:
	std::vector<Observer*> m_pObservers;
};

ObserverManager::ObserverManagerImpl::ObserverManagerImpl()
	:m_pObservers()
{

}

ObserverManager::ObserverManagerImpl::~ObserverManagerImpl()
{
	for (auto pObs : m_pObservers)
	{
		delete pObs;
	}
	m_pObservers.clear();
}

void ObserverManager::ObserverManagerImpl::AddObserver(Observer* pObserver)
{
	m_pObservers.emplace_back(pObserver);
}


ObserverManager::ObserverManager()
	:Singleton<ObserverManager>(),
	m_pImpl(new ObserverManagerImpl{})
{
	
}

ObserverManager::~ObserverManager()
{

}

void ObserverManager::AddObserver(Observer* pObserver)
{
	m_pImpl->AddObserver(pObserver);
}