#pragma once
#include "Entity.h"
#include "Component.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include <memory>

class JSonReader;
class StreamWriter;
class Registry final
{
public:
	Registry();

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	template <ComponentDerived T> T* const AddComponent(Entity entity);
	template <ComponentDerived T> void RemoveComponent(Entity entity);
	template <ComponentDerived T> T* const GetComponent(Entity entity) const;
	template <ComponentDerived T> std::vector<T*> GetComponents(Entity entity) const;
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
	int GetLivingEntitiesCount() const;
	Entity GetEntityAtIndex(int idx) const;

	void TransferTags(Entity originEntity, Entity destinationEntity, Registry* const pOther);

	void SerializeEntities(StreamWriter& writer);
	void DeserializeEntities(JsonReader const* reader, SerializationMap& context);
	void RestoreEntitiesContext(JsonReader const* reader, SerializationMap const& context);

	System* const AddSystemFromHash(size_t type);

private:
	std::unique_ptr<ComponentManager> m_pComponentManager;
	std::unique_ptr<EntityManager> m_pEntityManager;
	std::unique_ptr<SystemManager> m_pSystemManager;

	template <SystemDerived T> void OnSystemSignatureChanged(Signature const& signature);

	void DeserializeComponents(Entity entity, JsonReader const* reader, SerializationMap& context);
};

#include "Registry.inl"