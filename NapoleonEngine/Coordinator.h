#pragma once
#include "Entity.h"
#include "Component.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "BehaviourComponent.h"

#include <memory>

template<typename T>
concept BehaviourDerived = std::derived_from<T, BehaviourComponent>;

template<typename T>
concept ComponentDerivedNotBehaviour = std::derived_from<T, ECS_Component> && !std::derived_from<T, BehaviourComponent>;

class JSonReader;
class Coordinator final
{
public:
	Coordinator();

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	ComponentManager* const GetComponentManager() const;

	template <ComponentDerivedNotBehaviour T> std::shared_ptr<T> AddComponent(Entity entity);
	template <BehaviourDerived T> std::shared_ptr<T> AddComponent(Entity entity);
	template <ComponentDerived T> void RemoveComponent(Entity entity);
	template <ComponentDerived T> std::shared_ptr<T> GetComponent(Entity entity) const;
	template <ComponentDerived T> std::shared_ptr<T> FindComponentOfType() const;
	template <ComponentDerived T> std::shared_ptr<T> GetComponentInChildren(Entity entity) const;
	template <ComponentDerived T> ComponentType GetComponentType() const;
	template <SystemDerived T> std::shared_ptr<T> RegisterSystem();
	template <SystemDerived T> void SetSystemSignature(Signature signature);

	std::unordered_set<Entity> const& GetChildren(Entity entity) const;
	std::vector<Entity> GetEntityHierarchy(Entity entity) const;
	std::vector<std::shared_ptr<ECS_Component>> GetComponents(Entity entity);
	
	void SetEntityActive(Entity entity, bool isActive);
	void SetEntityHierarchyActive(Entity entity, bool isActive);
	void AddChild(Entity parent, Entity child);
	void SetTag(Entity entity, std::string const& tag);
	std::string GetTag(Entity entity) const;
	bool HasTag(Entity entity, std::string const& tag) const;

	void TransferTags(Entity originEntity, Entity destinationEntity, std::shared_ptr<Coordinator> pOther);

	void DeserializeComponents(Entity entity, JsonReader const* reader, SerializationMap& context);

private:
	std::unique_ptr<ComponentManager> m_pComponentManager;
	std::unique_ptr<EntityManager> m_pEntityManager;
	std::unique_ptr<SystemManager> m_pSystemManager;

	template <SystemDerived T> void OnSystemSignatureChanged(Signature const& signature);
};

template <ComponentDerivedNotBehaviour T>
std::shared_ptr<T> Coordinator::AddComponent(Entity entity)
{
	std::shared_ptr<T> comp = m_pComponentManager->AddComponent<T>(entity);

	Signature signature = m_pEntityManager->GetSignature(entity);

	signature.set(m_pComponentManager->GetComponentType<T>(), true);

	m_pEntityManager->SetSignature(entity, signature);

	m_pSystemManager->EntitySignatureChanged(entity, signature);

	return comp;
}

template <BehaviourDerived T> 
std::shared_ptr<T> Coordinator::AddComponent(Entity entity)
{
	std::shared_ptr<T> comp = m_pComponentManager->AddComponent<T>(entity);
	m_pComponentManager->RegisterComponent<BehaviourComponent>();
	m_pComponentManager->m_ComponentArrays[typeid(BehaviourComponent).name()]->ForceInsertData(comp, entity);

	Signature signature = m_pEntityManager->GetSignature(entity);
	signature.set(m_pComponentManager->GetComponentType<BehaviourComponent>(), true);

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
std::shared_ptr<T> Coordinator::GetComponent(Entity entity) const
{
	return m_pComponentManager->GetComponent<T>(entity);
}

template <ComponentDerived T> 
std::shared_ptr<T> Coordinator::FindComponentOfType() const
{
	return m_pComponentManager->FindComponentOfType<T>();
}

template <ComponentDerived T> 
std::shared_ptr<T> Coordinator::GetComponentInChildren(Entity entity) const
{
	std::shared_ptr<T> compToReturn;

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
std::shared_ptr<T> Coordinator::RegisterSystem()
{
	std::shared_ptr<T> pSystem = m_pSystemManager->RegisterSystem<T>(this);
	OnSystemSignatureChanged<T>(m_pSystemManager->GetSystemSignature<T>());
	return pSystem;
}

template <SystemDerived T>
void Coordinator::SetSystemSignature(Signature signature)
{
	m_pSystemManager->SetSignature<T>(signature);
	OnSystemSignatureChanged<T>(signature);
}

template <SystemDerived T>
void Coordinator::OnSystemSignatureChanged(Signature const& signature)
{
	std::vector<Entity> entities = m_pEntityManager->GetEntitiesWithSignature(signature);

	m_pSystemManager->AssignEntitiesToSystem<T>(entities);
}