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

	ComponentManager* const GetComponentManager() const;

	template <ComponentDerived T> void AddComponent(Entity entity, T const& component);
	template <ComponentDerived T> void RemoveComponent(Entity entity);
	template <ComponentDerived T> std::weak_ptr<T> GetComponent(Entity entity) const;
	template <ComponentDerived T> ComponentType GetComponentType() const;
	template <SystemDerived T> std::shared_ptr<T> RegisterSystem();
	template <SystemDerived T> void SetSystemSignature(Signature signature);

	std::unordered_set<Entity> const& GetChildren(Entity entity);
	std::vector<std::shared_ptr<ECS_Component>> GetComponents(Entity entity);
	
	void SetEntityActive(Entity entity, bool isActive);
	void SetEntityHierarchyActive(Entity entity, bool isActive);
	void AddChild(Entity parent, Entity child);
	void SetTag(Entity entity, std::string const& tag);
	std::string GetTag(Entity entity) const;
	bool HasTag(Entity entity, std::string const& tag) const;

	std::vector<std::shared_ptr<System>> ExtractSystems(std::shared_ptr<Coordinator> pOther);
	void TransferComponents(Entity originEntity, Entity destinationEntity, std::shared_ptr<Coordinator> pOther);
	void TransferTags(Entity originEntity, Entity destinationEntity, std::shared_ptr<Coordinator> pOther);

	void DeserializeComponents(Entity entity, JsonReader const* reader, SerializationMap& context);
	std::shared_ptr<System> AddSystemFromName(std::string const& str);

private:
	std::unique_ptr<ComponentManager> m_pComponentManager;
	std::unique_ptr<EntityManager> m_pEntityManager;
	std::unique_ptr<SystemManager> m_pSystemManager;

	template <SystemDerived T> void OnSystemSignatureChanged(Signature const& signature);
};

template <ComponentDerived T>
void Coordinator::AddComponent(Entity entity, T const& component)
{
	if (dynamic_cast<const ECS_Component*>(&component) == nullptr)
	{
		const char* typeName = typeid(T).name();
		Debugger::GetInstance().LogError("Components must inherit from ECS_Component. Can't add " + std::string(typeName) + " as component");
	}

	m_pComponentManager->AddComponent<T>(entity, component);

	Signature signature = m_pEntityManager->GetSignature(entity);
	signature.set(m_pComponentManager->GetComponentType<T>(), true);
	m_pEntityManager->SetSignature(entity, signature);

	m_pSystemManager->EntitySignatureChanged(entity, signature);
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
std::weak_ptr<T> Coordinator::GetComponent(Entity entity) const
{
	return m_pComponentManager->GetComponent<T>(entity);
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