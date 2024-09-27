#pragma once
#include "SystemManager.h"

template <SystemDerived T>
T* const SystemManager::RegisterSystem(Coordinator* const pRegistry)
{
	size_t type = std::type_index(typeid(T)).hash_code();

	if (m_Systems.find(type) != m_Systems.end())
	{
		Debugger::Get().LogWarning("System with hash code " + std::to_string(type) + " already registered");
		return nullptr;
	}

	auto pSystem = new T{};
	m_Systems.insert(std::make_pair(type, std::unique_ptr<T>(pSystem)));

	return pSystem;
}

template <SystemDerived T>
void SystemManager::SetSignature(Signature signature)
{
	size_t type = std::type_index(typeid(T)).hash_code();

	assert(m_Systems.find(type) != m_Systems.end() && "Trying to use a system not registered yet");

	m_Signatures[type] = signature;
}

template <SystemDerived T>
void SystemManager::UpdateSignature(Signature signature)
{
	size_t type = std::type_index(typeid(T)).hash_code();

	if (m_Systems.find(type) == m_Systems.end())
	{
		//TODO : LOG
		RegisterSystem<T>(signature);
	}
	else
	{
		m_Signatures.at(type) = signature;
	}
}

template <SystemDerived T>
Signature const& SystemManager::GetSystemSignature() const
{
	size_t type = std::type_index(typeid(T)).hash_code();

	assert(m_Systems.find(type) != m_Systems.end() && "Trying to use a system not registered yet");

	return m_Signatures.at(type);
}

template <SystemDerived T>
void SystemManager::AssignEntitiesToSystem(std::vector<Entity> const& entities) const
{
	if (entities.empty())
	{
		return;
	}
	size_t type = std::type_index(typeid(T)).hash_code();

	assert(m_Systems.find(type) != m_Systems.end() && "Trying to use a system not registered yet");

	System* const pSystem = m_Systems.at(type).get();

	if (pSystem == nullptr)
	{
		return;
	}

	for (Entity entity : entities)
	{
		pSystem->m_Entities.insert(entity);
	}
}

template <SystemDerived T>
T* const SystemManager::GetSystem() const
{
	size_t type = std::type_index(typeid(T)).hash_code();

	auto systemIt = m_Systems.find(type);

	if (systemIt != m_Systems.end())
	{
		return static_cast<T*>(systemIt->second.get());
	}

	return nullptr;
}