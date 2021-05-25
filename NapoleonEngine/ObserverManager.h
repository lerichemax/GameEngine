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
		
		template <typename T>
		Observer* GetObserver() const;
	private:
		friend class Singleton<ObserverManager>;
		ObserverManager() = default;

		std::vector<Observer*> m_pObservers;
	};

	template <class T>
	Observer* ObserverManager::GetObserver() const
	{
		type_info const& type = typeid(T);
		for (Observer* pObs : m_pObservers)
		{
			if (typeid(*pObs) == type)
			{
				return pObs;
				//return static_cast<T*>(pObs);
			}
		}
		Debugger::GetInstance().Log("Observer not found");
		return nullptr;
	}
}
