#include "PCH.h"
#include "Factories.h"

std::shared_ptr<ECS_Component> ComponentFactory::Create(std::string const& typeName, ComponentManager* const compManager)
{
	assert(m_Creators.find(typeName) != m_Creators.end() && "Component type not registered for creation");

	return m_Creators.at(typeName)(compManager);
}

std::shared_ptr<Command> CommandFactory::Create(std::string const& typeName)
{
	if (typeName.empty())
	{
		return nullptr;
	}

	assert(m_Creators.find(typeName) != m_Creators.end() && "Component type not registered for creation");

	return m_Creators.at(typeName)();
}