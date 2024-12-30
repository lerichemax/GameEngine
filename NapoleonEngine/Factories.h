#pragma once
#include "Singleton.h"

#include <functional>
#include <unordered_map>
#include <string>
#include <typeindex>


class Factory final : public Singleton<Factory>
{
	using Creator = std::function<void* ()>;

public:
	template <typename T>
	void RegisterType(Creator creator)
	{
		std::string type{ typeid(T).name() };

		m_Creators[type] = creator;
	}

	template <typename T>
	T* Create(std::string type)
	{
		std::string errorMsg{ "Component " + type + " not registered for creation." };

		assert(m_Creators.find(type) != m_Creators.end() && errorMsg.c_str());

		return static_cast<T*>(m_Creators.at(type)());
	}

private:
	std::unordered_map <std::string, Creator> m_Creators;
};