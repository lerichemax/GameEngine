#include "PCH.h"
#include "Reflection.h"

ecs::Component* const Reflection::CreateInstance(std::string const& typeName)
{
	auto it = m_ClassRegistry.find(typeName);
	if (it != m_ClassRegistry.end())
	{
		return it->second->CreateInstance();
	}

	return nullptr;
}
