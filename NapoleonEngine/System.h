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
	std::set<Entity> m_Entities;
};