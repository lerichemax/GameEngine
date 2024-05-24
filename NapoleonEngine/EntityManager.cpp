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
		if ((m_Signatures[i] & signature) == signature)
		{
			entities.push_back(i);
		}
	}

	return entities;
}

std::unordered_set<Entity> const& EntityManager::GetChildren(Entity entity)
{
	return m_EntitiesHierarchy.at(entity);
}

bool EntityManager::IsEntityValid(Entity entity)
{
	return entity <= MAX_ENTITIES;
}