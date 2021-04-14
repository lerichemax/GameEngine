#pragma once
#include "Singleton.h"
#include "Observer.h"
#include <map>

namespace empire
{
	class ObserverManager final : public Singleton<ObserverManager>
	{
	public:
		ObserverManager(ObserverManager const& other) = delete;
		ObserverManager(ObserverManager&& other) = delete;
		ObserverManager& operator=(ObserverManager const& rhs) = delete;
		ObserverManager& operator=(ObserverManager&& rhs) = delete;
		~ObserverManager();
		
		void AddObserver(unsigned int id, Observer* pObserver);
		Observer* GetObserver(unsigned int id) { return m_pObservers.at(id); }
	private:
		friend class Singleton<ObserverManager>;
		ObserverManager() = default;

		std::map<unsigned int, Observer*> m_pObservers;
	};
}
