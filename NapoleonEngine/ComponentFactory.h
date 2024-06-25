#pragma once
#include "Singleton.h"
#include "ComponentArray.h"

#include <functional>
#include <unordered_map>
#include <string>

class ComponentManager;
struct ECS_Component;
class ComponentFactory : public Singleton<ComponentFactory>
{
	using Creator = std::function<std::shared_ptr<ECS_Component>(ComponentManager* const compManager)>;

public:
	template <ComponentDerived T> void RegisterType(Creator creator);
	std::shared_ptr<ECS_Component> Create(std::string const& typeName, ComponentManager* const compManager);

private:
	std::unordered_map<std::string, Creator> m_Creators;
};

template <ComponentDerived T>
void ComponentFactory::RegisterType(Creator creator)
{
	std::string typeName{ typeid(T).name() };
	m_Creators[typeName] = creator;
}