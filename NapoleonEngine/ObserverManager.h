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
		
		void AddObserver(Observer* pObserver);
		
	private:
		friend class Singleton<ObserverManager>;
		ObserverManager();

		class ObserverManagerImpl;
		std::unique_ptr<ObserverManagerImpl> m_pImpl;
		

	};
}
