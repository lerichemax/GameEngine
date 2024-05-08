#include "PCH.h"
#include "SystemManager.h"

void SystemManager::EntityDestroyed(Entity entity)
{
	for (std::pair<const char*, std::shared_ptr<System>> const& pair : m_Systems)
	{
		std::shared_ptr<System> const& pSystem = pair.second;

		pSystem->m_Entities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (std::pair<const char*, std::shared_ptr<System>> const& pair : m_Systems)
	{
		auto const& type = pair.first;
		std::shared_ptr<System> const& pSystem = pair.second;
		Signature const& systemSignature = m_Signatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			pSystem->m_Entities.insert(entity);
		}
		else {
			pSystem->m_Entities.erase(entity);
		}
	}
}