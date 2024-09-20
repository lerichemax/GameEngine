#pragma once
#include "Component.h"
#include "Entity.h"
#include "System.h"

#include <unordered_map>
#include <memory>
#include <concepts>
#include <typeindex>

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

private:
	friend class Coordinator;

	std::unordered_map<size_t, Signature> m_Signatures;
	std::unordered_map<size_t, std::unique_ptr<System>> m_Systems;

	bool ForceAddSystem(size_t type, System* system);
	System* const GetSystemFromType(size_t type) const;
};

#include "SystemManager.inl"