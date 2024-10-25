#include "PCH.h"
#include "SystemManager.h"

#include "FPSCounter.h"

void SystemManager::EntityDestroyed(Entity entity)
{
	for (std::pair<size_t const, std::unique_ptr<System>> const& pair : m_Systems)
	{
		pair.second->m_Entities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature const& entitySignature)
{
	for (std::pair<size_t const, std::unique_ptr<System>> const& pair : m_Systems)
	{
		auto const& type = pair.first;
		Signature const& systemSignature = m_Signatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			pair.second->AddEntity(entity);
		}
		else {
			pair.second->m_Entities.erase(entity);
		}
	}
}

bool SystemManager::ForceAddSystem(size_t type, System* system)
{
	if (m_Systems.find(type) != m_Systems.end())
	{
		LOG_WARNING("A System with Hash type %lld already exists", type);
		return false;
	}

	m_Systems.insert(std::make_pair(type, std::unique_ptr<System>(std::move(system))));
	return true;
}

System* const SystemManager::GetSystemFromType(size_t type) const
{
	if (m_Systems.find(type) != m_Systems.end())
	{
		return m_Systems.at(type).get();
	}

	return nullptr;
}