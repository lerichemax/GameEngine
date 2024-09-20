#pragma once
#include "Coordinator.h"

template <ComponentDerived T>
T* const Coordinator::AddComponent(Entity entity)
{
	T* const comp = m_pComponentManager->AddComponent<T>(entity);

	Signature signature = m_pEntityManager->GetSignature(entity);

	signature.set(m_pComponentManager->GetComponentType<T>(), true);

	m_pEntityManager->SetSignature(entity, signature);

	m_pSystemManager->EntitySignatureChanged(entity, signature);

	return comp;
}

template <ComponentDerived T>
void Coordinator::RemoveComponent(Entity entity)
{
	m_pComponentManager->RemoveComponent<T>(entity);

	Signature signature = m_pEntityManager->GetSignature(entity);
	signature.set(m_pComponentManager->GetComponentType<T>(), false);
	m_pEntityManager->SetSignature(entity, signature);

	m_pSystemManager->EntitySignatureChanged(entity, signature);
}

template <ComponentDerived T>
T* const Coordinator::GetComponent(Entity entity) const
{
	return m_pComponentManager->GetComponent<T>(entity);
}

template<ComponentDerived T>
std::vector<T*> Coordinator::GetComponents(Entity entity) const
{
	return m_pComponentManager->GetComponents<T>(entity);
}

template <ComponentDerived T>
T* const Coordinator::FindComponentOfType() const
{
	return m_pComponentManager->FindComponentOfType<T>();
}

template <ComponentDerived T>
T* const Coordinator::GetComponentInChildren(Entity entity) const
{
	T* compToReturn;

	for (Entity child : GetChildren(entity))
	{
		compToReturn = m_pComponentManager->GetComponent<T>(child);

		if (compToReturn != nullptr)
		{
			return compToReturn;
		}
	}

	return nullptr;
}

template <ComponentDerived T>
ComponentType Coordinator::GetComponentType() const
{
	return m_pComponentManager->GetComponentType<T>();
}

template <SystemDerived T>
T* const Coordinator::RegisterSystem()
{
	auto pSystem = m_pSystemManager->RegisterSystem<T>(this);
	pSystem->m_pRegistry = this;
	pSystem->SetSignature();

	OnSystemSignatureChanged<T>(m_pSystemManager->GetSystemSignature<T>());

	Factory<System>::GetInstance().RegisterType<T>([]() {
		return new T{};
		});

	return pSystem;
}

template <SystemDerived T>
void Coordinator::SetSystemSignature(Signature signature)
{
	m_pSystemManager->SetSignature<T>(signature);
	OnSystemSignatureChanged<T>(signature);
}

template <SystemDerived T>
T* const Coordinator::GetSystem() const
{
	return m_pSystemManager->GetSystem<T>();
}

template <SystemDerived T>
void Coordinator::OnSystemSignatureChanged(Signature const& signature)
{
	std::vector<Entity> entities = m_pEntityManager->GetEntitiesWithSignature(signature);

	m_pSystemManager->AssignEntitiesToSystem<T>(entities);
}