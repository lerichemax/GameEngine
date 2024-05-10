#pragma once
#include "Entity.h"
#include "Component.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include <memory>

class Coordinator final
{
public:
	Coordinator();

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	ComponentManager* const GetComponentManager() const;

	template <typename T> void AddComponent(Entity entity, T const& component);
	template <typename T> void RemoveComponent(Entity entity);
	template <typename T> std::weak_ptr<T> GetComponent(Entity entity) const;
	template <typename T> ComponentType GetComponentType() const;
	template <typename T> std::shared_ptr<T> RegisterSystem(Signature signature);
	template <typename T> std::shared_ptr<T> RegisterSystem();
	template <typename T> void SetSystemSignature(Signature signature);

private:
	std::unique_ptr<ComponentManager> m_ComponentManager;
	std::unique_ptr<EntityManager> m_EntityManager;
	std::unique_ptr<SystemManager> m_SystemManager;

	template <typename T> void OnSystemSignatureChanged(Signature const& signature);
};

template <typename T> 
void Coordinator::AddComponent(Entity entity, T const& component)
{
	m_ComponentManager->AddComponent<T>(entity, component);

	Signature signature = m_EntityManager->GetSignature(entity);
	signature.set(m_ComponentManager->GetComponentType<T>(), true);
	m_EntityManager->SetSignature(entity, signature);

	m_SystemManager->EntitySignatureChanged(entity, signature);
}

template <typename T> 
void Coordinator::RemoveComponent(Entity entity)
{
	m_ComponentManager->RemoveComponent<T>(entity);

	Signature signature = m_EntityManager->GetSignature(entity);
	signature.set(m_ComponentManager->GetComponentType<T>(), false);
	m_EntityManager->SetSignature(entity, signature);

	m_SystemManager->EntitySignatureChanged(entity, signature);
}

template <typename T> 
std::weak_ptr<T> Coordinator::GetComponent(Entity entity) const
{
	return m_ComponentManager->GetComponent<T>(entity);
}

template <typename T> 
ComponentType Coordinator::GetComponentType() const
{
	return m_ComponentManager->GetComponentType<T>();
}

template <typename T> 
std::shared_ptr<T> Coordinator::RegisterSystem(Signature signature)
{
	std::shared_ptr<T> pSystem = m_SystemManager->RegisterSystem<T>(signature, this);
	OnSystemSignatureChanged<T>(signature);
	return pSystem;
}

template <typename T>
std::shared_ptr<T> Coordinator::RegisterSystem()
{
	std::shared_ptr<T> pSystem = m_SystemManager->RegisterSystem<T>(this);
	OnSystemSignatureChanged<T>(m_SystemManager->GetSystemSignature<T>());
	return pSystem;
}

template <typename T> 
void Coordinator::SetSystemSignature(Signature signature)
{
	m_SystemManager->SetSignature<T>(signature);
	OnSystemSignatureChanged<T>(signature);
}

template <typename T>
void Coordinator::OnSystemSignatureChanged(Signature const& signature)
{
	std::vector<Entity> entities = m_EntityManager->GetEntitiesWithSignature(signature);

	m_SystemManager->AssignEntitiesToSystem<T>(entities);
}