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
	template <SystemDerived T> T* const RegisterSystem(Coordinator* const pRegistry);
	template <SystemDerived T> void SetSignature(Signature signature);
	template <SystemDerived T> void UpdateSignature(Signature signature);
	template <SystemDerived T> Signature const& GetSystemSignature() const;
	template <SystemDerived T> void AssignEntitiesToSystem(std::vector<Entity> const& entities) const;
	template <SystemDerived T> T* const GetSystem() const;

	void EntityDestroyed(Entity entity);
	void EntitySignatureChanged(Entity entity, Signature const& entitySignature);

	System* const ForceAddSystem(std::string name, System* system);

private:
	friend class Coordinator;

	std::unordered_map<std::string, Signature> m_Signatures;
	std::unordered_map<std::string, std::unique_ptr<System>> m_Systems;
};

template <SystemDerived T>
T* const SystemManager::RegisterSystem(Coordinator* const pRegistry)
{
	const char* typeName = typeid(T).name();

	assert(m_Systems.find(typeName) == m_Systems.end() && "Trying to register a system more than once");

	auto pSystem = new T{};
	m_Systems.insert(std::make_pair(typeName, std::unique_ptr<T>(pSystem)));
	pSystem->SetSignature(pRegistry);

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

	System* const pSystem = m_Systems.at(typeName).get();

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
	const char* typeName = typeid(T).name();

	auto systemIt = m_Systems.find(typeName);

	if (systemIt != m_Systems.end())
	{
		return static_cast<T*>(systemIt->second.get());
	}

	return nullptr;
}