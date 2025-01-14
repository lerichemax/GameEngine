#include "PCH.h"
#include "EntityManager.h"

EntityManager::EntityManager()
{
	for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
	{
		m_AvailableEntities.push(entity);
	}
}

Entity EntityManager::CreateEntity()
{
	assert(m_LivingEntitiesCount < MAX_ENTITIES && "Too many entities in existence");

	Entity id = m_AvailableEntities.front();
	m_CreatedEntities.push_back(id);
	m_AvailableEntities.pop();
	m_LivingEntitiesCount++;

	m_EntitiesHierarchy.insert(std::make_pair(id, std::unordered_set<Entity>{}));

	return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range");

	m_Signatures[entity].reset();

	m_AvailableEntities.push(entity);

	m_EntitiesHierarchy.erase(entity);

	m_CreatedEntities.erase(std::remove(m_CreatedEntities.begin(), m_CreatedEntities.end(), entity), m_CreatedEntities.end());

	m_LivingEntitiesCount--;
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
	assert(entity < MAX_ENTITIES && "Entity out of range");

	m_Signatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range");

	return m_Signatures[entity];
}

void EntityManager::AddChild(Entity parent, Entity child)
{
	m_EntitiesHierarchy.at(parent).insert(child);
}

std::vector<Entity> EntityManager::GetEntitiesWithSignature(Signature const& signature)
{
	std::vector<Entity> entities;

	for (Entity i = 0; i < m_LivingEntitiesCount; i++)
	{
		if (EntityHasSignature(i, signature))
		{
			entities.push_back(i);
		}
	}

	return entities;
}

std::unordered_set<Entity> const& EntityManager::GetChildren(Entity entity) const
{
	return m_EntitiesHierarchy.at(entity);
}

std::vector<Entity> EntityManager::GetEntityHierarchy(Entity entity) const //TODO: modify to avoid multiple vector copies.
{
	std::vector<Entity> hierarchy;
	hierarchy.push_back(entity);

	for (Entity child : GetChildren(entity))
	{
		std::vector childHierarchy = GetEntityHierarchy(child);
		hierarchy.insert(childHierarchy.begin(), childHierarchy.end(), hierarchy.end());
	}
	return hierarchy;
}

void EntityManager::SetTag(Entity entity, std::string const& tag)
{
	m_EntityToTag.insert(std::make_pair(entity, tag));
}

std::string EntityManager::GetTag(Entity entity) const
{
	auto it = m_EntityToTag.find(entity);

	if (it == m_EntityToTag.end())
	{
		return "";
	}
	
	return it->second;
}

bool EntityManager::EntityHasTag(Entity entity, std::string const& tag) const
{
	if (!EntityHasATag(entity))
	{
		return false;
	}

	return m_EntityToTag.at(entity) == tag;
}

bool EntityManager::EntityHasATag(Entity entity) const
{
	auto it = m_EntityToTag.find(entity);

	return it != m_EntityToTag.end();
}

bool EntityManager::EntityHasSignature(Entity entity, Signature signature) const
{
	return (m_Signatures[entity] & signature) == signature;
}

bool EntityManager::IsEntityValid(Entity entity)
{
	return entity <= MAX_ENTITIES;
}