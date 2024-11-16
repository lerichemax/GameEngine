#pragma once
#include "Singleton.h"
#include "ComponentArray.h"

#include <functional>
#include <unordered_map>
#include <string>
#include <typeindex>


template <class D, class B>
concept Derived = std::derived_from<D, B>;

template <class T>
class Factory final : public Singleton<Factory<T>>
{
	using Creator = std::function<T*()>;

public:
	template <Derived<T> D> 
	void RegisterType(Creator creator)
	{
		std::string type{ typeid(D).name() };

		m_Creators[type] = creator;
	}

	T* Create(std::string type)
	{
		std::string errorMsg{ "Component " + type + " not registered for creation."};

		assert(m_Creators.find(type) != m_Creators.end() && errorMsg.c_str());

		return m_Creators.at(type)();
	}

private:
	std::unordered_map <std::string, Creator> m_Creators;
};