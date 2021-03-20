#pragma once

namespace empire
{
	template <typename T>
	class ServiceLocator final
	{
	public:
		
		static T& GetService() { return *m_pService; }
		static void Register(T* p) { m_pService = p == nullptr ? &m_NullService : p; }

	private:
		static T* m_pService;
		static T m_NullService;
	};
}

template <typename T>
T empire::ServiceLocator<T>::m_NullService{};

template <typename T>
T* empire::ServiceLocator<T>::m_pService = &m_NullService;

