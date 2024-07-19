#pragma once
#include "Component.h"
#include "Entity.h"
#include "System.h"

#include <unordered_map>
#include <memory>
#include <concepts>

template <typename T>
concept SystemDerived = std::derived_from<T, System>;

class ComponentManager;
class Coordinator;
class SystemManager
{
public:
	template <SystemDerived T> std::shared_ptr<T> RegisterSystem(Coordinator* const pRegistry);
	template <SystemDerived T> void SetSignature(Signature signature);
	template <SystemDerived T> void UpdateSignature(Signature signature);
	template <SystemDerived T> Signature const& GetSystemSignature() const;
	template <SystemDerived T> void AssignEntitiesToSystem(std::vector<Entity> const& entities) const;

	void EntityDestroyed(Entity entity);
	void EntitySignatureChanged(Entity entity, Signature const& entitySignature);

private:
	friend class Coordinator;

	std::unordered_map<std::string, Signature> m_Signatures;
	std::unordered_map<std::string, std::shared_ptr<System>> m_Systems;

	std::shared_ptr<System> AddSystemFromName(std::string const& str, Coordinator* const pRegistry);
	std::shared_ptr<System> CreateSystemFromName(std::string const& str) const;
};

template <SystemDerived T>
std::shared_ptr<T> SystemManager::RegisterSystem(Coordinator* const pRegistry)
{
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) == m_Systems.end() && "Trying to register a system more than once");

	m_Systems.insert(std::make_pair(typeName, nullptr));
	auto pSystem = std::make_shared<T>();
	pSystem->SetSignature(pRegistry);
	m_Systems.at(typeName) = pSystem;

	return pSystem;
}

template <SystemDerived T>
void SystemManager::SetSignature(Signature signature)
{
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) != m_Systems.end() && "Trying to use a system not registered yet");

	m_Signatures[typeName] = signature;
}

template <SystemDerived T>
void SystemManager::UpdateSignature(Signature signature)
{
	const char* typeName = typeid(T).name();

	if (m_Systems.find(typeName) == m_Systems.end())
	{
		//TODO : LOG
		RegisterSystem<T>(signature);
	}
	else
	{
		m_Signatures.at(typeName) = signature;
	}
}

template <SystemDerived T>
Signature const& SystemManager::GetSystemSignature() const
{
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) != m_Systems.end() && "Trying to use a system not registered yet");

	return m_Signatures.at(typeName);
}

template <SystemDerived T>
void SystemManager::AssignEntitiesToSystem(std::vector<Entity> const& entities) const
{
	if (entities.empty())
	{
		return;
	}
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) != m_Systems.end() && "Trying to use a system not registered yet");

	std::shared_ptr<System> pSystem = m_Systems.at(typeName);

	if (pSystem == nullptr)
	{
		return;
	}

	for (Entity entity : entities)
	{
		pSystem->m_Entities.insert(entity);
	}
}