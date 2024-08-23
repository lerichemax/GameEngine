#pragma once
#include "Singleton.h"
#include "ComponentArray.h"

#include <functional>
#include <unordered_map>
#include <string>


template <class D, class B>
concept Derived = std::derived_from<D, B>;

template <class T, typename... params>
class Factory final : public Singleton<Factory<T, params...>>
{
	using Creator = std::function<T*(params...)>;

public:
	template <Derived<T> D> 
	void RegisterType(Creator creator)
	{
		std::string typeName{ typeid(D).name() };
		m_Creators[typeName] = creator;
	}

	T* Create(std::string const& typeName, params... parameters)
	{
		if (typeName.empty())
		{
			return nullptr;
		}
		std::string errorMsg{ typeName + " not registered for creation" };

		assert(m_Creators.find(typeName) != m_Creators.end() && errorMsg.c_str());

		return m_Creators.at(typeName)(parameters...);
	}

private:
	std::unordered_map<std::string, Creator> m_Creators;
};