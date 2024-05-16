#pragma once
#include "Entity.h"
#include "Component.h"

#include <queue>
#include <array>

class EntityManager
{
public:
	EntityManager();
	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	void SetSignature(Entity entity, Signature signature);
	Signature GetSignature(Entity entity);
	std::vector<Entity> GetEntitiesWithSignature(Signature const&  signature);
	static bool IsEntityValid(Entity entity);

private:
	friend class Coordinator;

	std::queue<Entity> m_AvailableEntities;
	std::array<Signature, MAX_ENTITIES> m_Signatures;
	uint32_t m_LivingEntitiesCount;

};