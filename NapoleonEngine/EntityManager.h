#pragma once
#include "Entity.h"
#include "Component.h"

#include <queue>
#include <array>
#include <unordered_set>
#include <unordered_map>

class EntityManager
{
public:
	EntityManager();
	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	void SetSignature(Entity entity, Signature signature);
	Signature GetSignature(Entity entity);
	void AddChild(Entity parent, Entity child);
	std::vector<Entity> GetEntitiesWithSignature(Signature const&  signature);
	std::unordered_set<Entity> const& GetChildren(Entity entity) const;
	std::vector<Entity> GetEntityHierarchy(Entity entity) const; //not fully functional, make recursive
	void SetTag(Entity entity, std::string const& tag);
	std::string GetTag(Entity entity) const;
	bool HasTag(Entity entity, std::string const& tag) const;
	bool HasATag(Entity entity) const;

	static bool IsEntityValid(Entity entity);

private:
	friend class Coordinator;

	std::queue<Entity> m_AvailableEntities;
	std::array<Signature, MAX_ENTITIES> m_Signatures;
	std::unordered_map<Entity, std::unordered_set<Entity>> m_EntitiesHierarchy{};
	std::unordered_map<Entity, std::string> m_EntityToTag{};
	int32_t m_LivingEntitiesCount{};

	//bool HasChild(Entity entity, Entity child) const;
	//bool CheckHierarchyForExistingChild(Entity child);
};