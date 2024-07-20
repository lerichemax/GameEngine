#pragma once
#include "Component.h"
#include "ComponentArray.h"
#include "Entity.h"
#include "JsonReaderWriter.h"
#include "Factories.h"

#include <unordered_map>
#include <string>
#include <memory>

class ComponentManager
{
public:
	~ComponentManager() = default;

	template<ComponentDerived T> ComponentType GetComponentType();

	template<ComponentDerived T> std::shared_ptr<T> AddComponent(Entity entity, T const& component);

	template<ComponentDerived T> void RemoveComponent(Entity entity);
	template<ComponentDerived T> std::shared_ptr<T> GetComponent(Entity entity);
	template<ComponentDerived T> std::vector<std::shared_ptr<T>> GetComponents(Entity entity);
	template<ComponentDerived T> std::shared_ptr<T> FindComponentOfType();
	
	void EntityDestroyed(Entity entity);
	std::vector<std::shared_ptr<ECS_Component>> GetComponentsForSignature(Entity entity, Signature signature);

private:
	friend class Coordinator;

	static std::unordered_map<std::string, ComponentType> m_ComponentTypes;
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
	static ComponentType m_NextComponentType;

	template<ComponentDerived T> void RegisterComponent();
	template <ComponentDerived T> std::shared_ptr<ComponentArray<T>> GetComponentArray();

	ComponentType DeserializeAndAddComponent(Entity entity, JsonReader const* reader, SerializationMap& context);
	template<ComponentDerived T> void RegisterComponentArray(const char* typeName);
};

template<ComponentDerived T>
void ComponentManager::RegisterComponent()
{
	const char* typeName = typeid(T).name();

	if (m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
	{
		m_ComponentTypes.insert(std::make_pair(typeName, m_NextComponentType++));

		ComponentFactory::GetInstance().RegisterType<T>([](ComponentManager* const compManager) {
			auto shared = std::make_shared<T>();

			const char* compTypeName = typeid(T).name();

			compManager->RegisterComponentArray<T>(compTypeName);

			return shared;
		});
	}

	RegisterComponentArray<T>(typeName);
}

template<ComponentDerived T>
ComponentType ComponentManager::GetComponentType()
{
	RegisterComponent<T>();

	const char* typeName = typeid(T).name();

	return m_ComponentTypes[typeName];
}

template<ComponentDerived T>
std::shared_ptr<T> ComponentManager::AddComponent(Entity entity, T const& component)
{
	RegisterComponent<T>();
	return GetComponentArray<T>()->InsertData(entity, component);
}

template<ComponentDerived T>
void ComponentManager::RemoveComponent(Entity entity)
{
	return GetComponentArray<T>()->RemoveData(entity);
}

template<ComponentDerived T>
std::shared_ptr<T> ComponentManager::GetComponent(Entity entity)
{
	return GetComponentArray<T>()->GetData(entity);
}
template<ComponentDerived T>
std::vector<std::shared_ptr<T>> ComponentManager::GetComponents(Entity entity)
{
	std::vector<std::shared_ptr<T>> toReturn;

	for (auto compArray : m_ComponentArrays)
	{
		auto baseComp = compArray.second->GetBaseData(entity);
		if (baseComp != nullptr)
		{
			auto castedPtr = std::dynamic_pointer_cast<T>(baseComp);
			if (castedPtr != nullptr)
			{
				toReturn.push_back(castedPtr);
			}
		}
	}

	return toReturn;
}

template<ComponentDerived T> 
std::shared_ptr<T>  ComponentManager::FindComponentOfType()
{
	auto compArray = GetComponentArray<T>();

	if (compArray != nullptr)
	{
		return compArray->m_Components[0];
	}

	return nullptr;
}


template <ComponentDerived T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray()
{
	const char* typeName = typeid(T).name();

	assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component type not registered before use");

	return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
}

template<ComponentDerived T> 
void ComponentManager::RegisterComponentArray(const char* typeName)
{
	if (m_ComponentArrays.find(typeName) == m_ComponentArrays.end())
	{
		m_ComponentArrays.insert(std::make_pair(typeName, std::make_shared<ComponentArray<T>>()));
	}
}