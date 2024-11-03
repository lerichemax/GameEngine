#pragma once
#include "Entity.h"
#include "Component.h"

#include <queue>
#include <array>
#include <unordered_set>
#include <set>
#include <unordered_map>

class EntityManager
{
	template<ComponentDerived... Components> friend class View;

public:
	EntityManager();
	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	void SetSignature(Entity entity, Signature signature);
	Signature GetSignature(Entity entity);
	void AddChild(Entity parent, Entity child);
	std::vector<Entity> GetEntitiesWithSignature(Signature const&  signature);
	std::unordered_set<Entity> const& GetChildren(Entity entity) const;
	std::vector<Entity> GetEntityHierarchy(Entity entity) const;
	void SetTag(Entity entity, std::string const& tag);
	std::string GetTag(Entity entity) const;
	bool EntityHasTag(Entity entity, std::string const& tag) const;
	bool EntityHasATag(Entity entity) const;
	bool EntityHasSignature(Entity entity, Signature signature) const;

	static bool IsEntityValid(Entity entity);

private:
	friend class Registry;

	std::queue<Entity> m_AvailableEntities;
	std::vector<Entity> m_CreatedEntities;
	std::array<Signature, MAX_ENTITIES> m_Signatures;
	std::unordered_map<Entity, std::unordered_set<Entity>> m_EntitiesHierarchy{};
	std::unordered_map<Entity, std::string> m_EntityToTag{};
	int32_t m_LivingEntitiesCount{};
};