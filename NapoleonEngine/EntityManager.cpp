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

	return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range");

	m_Signatures[entity].reset();

	m_AvailableEntities.push(entity);
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