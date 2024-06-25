#include "PCH.h"
#include "ComponentFactory.h"

std::shared_ptr<ECS_Component> ComponentFactory::Create(std::string const& typeName, ComponentManager* const compManager)
{
	assert(m_Creators.find(typeName) != m_Creators.end() && "Component type not registered for creation");

	return m_Creators.at(typeName)(compManager);
}