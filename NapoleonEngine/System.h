#pragma once
#include "Entity.h"

#include <set>

class ComponentManager;
class System
{
	friend class SystemManager;

public:
	virtual void Update(ComponentManager* const pComponentManager) = 0;

protected:
	friend class Coordinator;

	std::set<Entity> m_Entities;

	virtual std::shared_ptr<System> Clone() const = 0; // we do not clone entities

	System() = default;
	System(System const& other);
};