#pragma once
#include "Entity.h"

#include <set>

class ComponentManager;
class Coordinator;
class System
{
public:
	
	virtual void Update(ComponentManager* const pComponentManager) = 0;
	virtual std::string GetNameForSerialization() const { return "Not for serialization"; };
	virtual void SetSignature(Coordinator* const pRegistry) = 0;

protected:
	friend class Coordinator;
	friend class SystemManager;

	std::set<Entity> m_Entities;

	virtual std::shared_ptr<System> Clone() const = 0; // we do not clone entities

	System() = default;
	System(System const& other);
};