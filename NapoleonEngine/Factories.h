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

class Command;

template<typename T>
concept CommandDerived = std::derived_from<T, Command>;

class CommandFactory : public Singleton<CommandFactory>
{
	using CommandCreator = std::function<std::shared_ptr<Command>()>;

public:
	template <CommandDerived T> void RegisterType(CommandCreator creator);
	std::shared_ptr<Command> Create(std::string const& typeName);

private:
	std::unordered_map<std::string, CommandCreator> m_Creators;
};

template <CommandDerived T>
void CommandFactory::RegisterType(CommandCreator creator)
{
	std::string typeName{ typeid(T).name() };
	m_Creators[typeName] = creator;
}