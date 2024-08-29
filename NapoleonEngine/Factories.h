#pragma once
#include "Singleton.h"
#include "ComponentArray.h"

#include <functional>
#include <unordered_map>
#include <string>
#include <typeindex>


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
		size_t type{ std::type_index(typeid(D)).hash_code() };

		m_Creators[type] = creator;
	}

	T* Create(size_t hashCode, params... parameters)
	{
		std::string errorMsg{ "Component with hash_code " + std::to_string(hashCode) + " not registered for creation."};

		assert(m_Creators.find(hashCode) != m_Creators.end() && errorMsg.c_str());

		return m_Creators.at(hashCode)(parameters...);
	}

private:
	std::unordered_map<size_t, Creator> m_Creators;
};