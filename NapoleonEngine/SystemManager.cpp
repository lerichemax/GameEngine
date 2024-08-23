#include "PCH.h"
#include "SystemManager.h"

#include "FPSCounter.h"

void SystemManager::EntityDestroyed(Entity entity)
{
	for (std::pair<std::string const, std::unique_ptr<System>> const& pair : m_Systems)
	{
		pair.second->m_Entities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature const& entitySignature)
{
	for (std::pair<std::string const, std::unique_ptr<System>> const& pair : m_Systems)
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

System* const SystemManager::ForceAddSystem(std::string name, System* system)
{
	if (m_Systems.find(name) != m_Systems.end())
	{
		Debugger::GetInstance().LogWarning(name + " already exists");
		return m_Systems.at(name).get();
	}

	m_Systems.insert(std::make_pair(name, std::unique_ptr<System>(system)));
	return m_Systems.at(name).get();
}