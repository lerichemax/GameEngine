#pragma once
#include "Component.h"
#include "Entity.h"
#include "System.h"

#include <unordered_map>
#include <memory>

class Coordinator;
class SystemManager
{
public:
	template <typename T> std::shared_ptr<T> RegisterSystem(Signature signature, Coordinator* const pRegistry);
	template <typename T> std::shared_ptr<T> RegisterSystem(Coordinator* const pRegistry);
	template <typename T> void SetSignature(Signature signature);
	template <typename T> void UpdateSignature(Signature signature);
	template <typename T> Signature const& GetSystemSignature() const;
	template <typename T> void AssignEntitiesToSystem(std::vector<Entity> const& entities) const;

	void EntityDestroyed(Entity entity);
	void EntitySignatureChanged(Entity entity, Signature const& entitySignature);

private:
	std::unordered_map<const char*, Signature> m_Signatures;
	std::unordered_map<const char*, std::shared_ptr<System>> m_Systems;
};

template <typename T> 
std::shared_ptr<T> SystemManager::RegisterSystem(Signature signature, Coordinator* const pRegistry)
{
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) == m_Systems.end() && "Trying to register a system more than once");

	m_Systems.insert(std::make_pair(typeName, nullptr));
	auto pSystem = std::make_shared<T>(pRegistry);
	m_Systems.at(typeName) = pSystem;

	m_Signatures.insert(std::make_pair(typeName, signature));
	return pSystem;
}

template <typename T>
std::shared_ptr<T> SystemManager::RegisterSystem(Coordinator* const pRegistry)
{
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) == m_Systems.end() && "Trying to register a system more than once");

	m_Systems.insert(std::make_pair(typeName, nullptr));
	auto pSystem = std::make_shared<T>(pRegistry);
	m_Systems.at(typeName) = pSystem;

	return pSystem;
}

template <typename T> 
void SystemManager::SetSignature(Signature signature)
{
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) != m_Systems.end() && "Trying to use a system not registered yet");

	m_Signatures.insert(std::make_pair(typeName, signature));
}

template <typename T> 
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

template <typename T> 
Signature const& SystemManager::GetSystemSignature() const
{
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) != m_Systems.end() && "Trying to use a system not registered yet");

	return m_Signatures.at(typeName);
}

template <typename T> 
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