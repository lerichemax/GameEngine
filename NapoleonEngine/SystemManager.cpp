#include "PCH.h"
#include "SystemManager.h"

#include "FPSCounter.h"

void SystemManager::EntityDestroyed(Entity entity)
{
	for (std::pair<std::string, std::shared_ptr<System>> const& pair : m_Systems)
	{
		std::shared_ptr<System> const& pSystem = pair.second;

		pSystem->m_Entities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature const& entitySignature)
{
	for (std::pair<std::string, std::shared_ptr<System>> const& pair : m_Systems)
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

std::shared_ptr<System> SystemManager::AddSystemFromName(std::string const& str, Coordinator* const pRegistry)
{
	if (str == "Not for serialization")
	{
		return nullptr;
	}

	m_Signatures.insert(std::make_pair(str, Signature{}));
	m_Systems.insert(std::make_pair(str, nullptr));

	auto pSystem = CreateSystemFromName(str);
	pSystem->SetSignature(pRegistry);
	if (pSystem != nullptr)
	{
		m_Systems[str] = pSystem;
	}

	return pSystem;
}

std::shared_ptr<System> SystemManager::CreateSystemFromName(std::string const& name) const
{
	if (name == "class FPSCounterSystem")
	{
		return std::static_pointer_cast<System>(std::make_shared<FPSCounterSystem>());
	}
	
	return nullptr;
}