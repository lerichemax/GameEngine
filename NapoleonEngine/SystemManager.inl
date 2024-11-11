#pragma once
#include "SystemManager.h"

namespace ecs
{
	template <SystemDerived T>
	T* const SystemManager::RegisterSystem(Registry* const pRegistry)
	{
		std::string type{ typeid(T).name() };

		if (m_Systems.find(type) != m_Systems.end())
		{
			LOG_WARNING("System %s already registered", type.c_str());
			return nullptr;
		}

		auto pSystem = new T{};
		m_Systems.insert(std::make_pair(type, std::unique_ptr<T>(pSystem)));

		LOG_WARNING("System %s registered", typeid(T).name());

		return pSystem;
	}

	template <SystemDerived T>
	T* const SystemManager::GetSystem() const
	{
		std::string type{ typeid(T).name() };

		auto systemIt = m_Systems.find(type);

		if (systemIt != m_Systems.end())
		{
			return static_cast<T*>(systemIt->second.get());
		}

		return nullptr;
	}
}
