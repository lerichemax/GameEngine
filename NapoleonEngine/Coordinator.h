#pragma once
#include "Entity.h"
#include "Component.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include <memory>

class JSonReader;
class Coordinator final
{
public:
	Coordinator();

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	template <ComponentDerived T> T* const AddComponent(Entity entity);
	template <ComponentDerived T> void RemoveComponent(Entity entity);
	template <ComponentDerived T> T* const GetComponent(Entity entity) const;
	template<ComponentDerived T> std::vector<T*> GetComponents(Entity entity) const;
	template <ComponentDerived T> T* const FindComponentOfType() const;
	template <ComponentDerived T> T* const GetComponentInChildren(Entity entity) const;
	template <ComponentDerived T> ComponentType GetComponentType() const;

	template <SystemDerived T> T* const RegisterSystem();
	template <SystemDerived T> void SetSystemSignature(Signature signature);
	template <SystemDerived T> T* const GetSystem() const;

	std::unordered_set<Entity> const& GetChildren(Entity entity) const;
	std::vector<Entity> GetEntityHierarchy(Entity entity) const;
	std::vector<Component*> GetEntityComponents(Entity entity);
	
	void SetEntityActive(Entity entity, bool isActive);
	void SetEntityHierarchyActive(Entity entity, bool isActive);
	void AddChild(Entity parent, Entity child);
	void SetTag(Entity entity, std::string const& tag);
	std::string GetTag(Entity entity) const;
	bool HasTag(Entity entity, std::string const& tag) const;

	void TransferTags(Entity originEntity, Entity destinationEntity, Coordinator* const pOther);

	void DeserializeComponents(Entity entity, JsonReader const* reader, SerializationMap& context);
	System* const AddSystemFromHash(size_t type);

private:
	std::unique_ptr<ComponentManager> m_pComponentManager;
	std::unique_ptr<EntityManager> m_pEntityManager;
	std::unique_ptr<SystemManager> m_pSystemManager;

	template <SystemDerived T> void OnSystemSignatureChanged(Signature const& signature);
};

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