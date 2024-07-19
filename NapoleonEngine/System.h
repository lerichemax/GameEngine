#pragma once
#include "Entity.h"

#include <set>

class ComponentManager;
class Coordinator;
class System
{
	friend class Coordinator;
	friend class SystemManager;

public:
	
	virtual void Update(ComponentManager* const pComponentManager) = 0;
	virtual std::string GetNameForSerialization() const { return "Not for serialization"; };
	virtual void SetSignature(Coordinator* const pRegistry) = 0;

protected:
	std::set<Entity> m_Entities;

	virtual void AddEntity(Entity entity);

	System() = default;
	System(System const& other);
	
};