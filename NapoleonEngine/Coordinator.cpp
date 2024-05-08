#include "PCH.h"
#include "Coordinator.h"

Coordinator::Coordinator()
	:m_ComponentManager{std::make_unique<ComponentManager>()},
	m_EntityManager{ std::make_unique<EntityManager>() },
	m_SystemManager{ std::make_unique<SystemManager>() }
{
}

Entity Coordinator::CreateEntity()
{
	return m_EntityManager->CreateEntity();
}

void Coordinator::DestroyEntity(Entity entity)
{
	m_EntityManager->DestroyEntity(entity);
	m_ComponentManager->EntityDestroyed(entity);
	m_SystemManager->EntityDestroyed(entity);
}

ComponentManager* const Coordinator::GetComponentManager() const
{
	return m_ComponentManager.get();
}