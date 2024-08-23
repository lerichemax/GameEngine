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

	template<ComponentDerived T> T* const AddComponent(Entity entity);

	template<ComponentDerived T> void RemoveComponent(Entity entity);
	template<ComponentDerived T> T* const GetComponent(Entity entity);
	template<ComponentDerived T> std::vector<T*> GetComponents(Entity entity);
	template<ComponentDerived T> T* const FindComponentOfType();
	
	void EntityDestroyed(Entity entity);
	std::vector<Component*> GetComponentsForSignature(Entity entity, Signature signature);

private:
	friend class Coordinator;

	static std::unordered_map<std::string, ComponentType> m_ComponentTypes;
	std::unordered_map<std::string, std::unique_ptr<IComponentArray>> m_ComponentArrays{};
	static ComponentType m_NextComponentType;

	template<ComponentDerived T> void RegisterComponent();
	template<ComponentDerived T> ComponentArray<T>* const GetComponentArray();
	template<ComponentDerived T> void RegisterComponentArray(std::string typeName);

	ComponentType DeserializeAndAddComponent(Entity entity, JsonReader const* reader, SerializationMap& context);
};

template<ComponentDerived T>
void ComponentManager::RegisterComponent()
{
	std::string typeName{ typeid(T).name() };

	if (m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
	{
		m_ComponentTypes.insert(std::make_pair(typeName, m_NextComponentType++));

		Factory<Component, ComponentManager* const>::GetInstance().RegisterType<T>([](ComponentManager* const compManager) {
			T* t = new T{};

			std::string compTypeName{ typeid(T).name() };

			compManager->RegisterComponentArray<T>(compTypeName);

			return t;
		});
	}

	RegisterComponentArray<T>(typeName);
}

template<ComponentDerived T>
ComponentType ComponentManager::GetComponentType()
{
	RegisterComponent<T>();

	std::string typeName{ typeid(T).name() };

	return m_ComponentTypes[typeName];
}

template<ComponentDerived T>
T* const ComponentManager::AddComponent(Entity entity)
{
	RegisterComponent<T>();
	return GetComponentArray<T>()->InsertData(entity);
}

template<ComponentDerived T>
void ComponentManager::RemoveComponent(Entity entity)
{
	return GetComponentArray<T>()->RemoveData(entity);
}

template<ComponentDerived T>
T* const ComponentManager::GetComponent(Entity entity)
{
	for (auto const& compArray : m_ComponentArrays)
	{
		Component* data = compArray.second->GetBaseData(entity);
		if (data != nullptr)
		{
			T* castedData = dynamic_cast<T*>(data);
			if (castedData != nullptr)
			{
				return castedData;
			}
		}
	}

	return nullptr;
}

template<ComponentDerived T>
std::vector<T*> ComponentManager::GetComponents(Entity entity)
{
	//return GetComponentArray<T>()->GetAllData(entity);
	std::vector<T*> components;

	for (auto const& compArray : m_ComponentArrays)
	{
		Component* data = compArray.second->GetBaseData(entity);
		if (data != nullptr)
		{
			T* castedData = dynamic_cast<T*>(data);
			if (castedData != nullptr)
			{
				components.push_back(castedData);
			}
		}
	}
	return components;
}

template<ComponentDerived T> 
T* const ComponentManager::FindComponentOfType()
{
	auto compArray = GetComponentArray<T>();

	if (compArray != nullptr)
	{
		return compArray->m_Components[0].get();
	}

	return nullptr;
}


template <ComponentDerived T>
ComponentArray<T>* const ComponentManager::GetComponentArray()
{
	std::string typeName{ typeid(T).name() };

	assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component type not registered before use");

	return static_cast<ComponentArray<T>*>(m_ComponentArrays[typeName].get());
}

template<ComponentDerived T> 
void ComponentManager::RegisterComponentArray(std::string typeName)
{
	if (m_ComponentArrays.find(typeName) == m_ComponentArrays.end())
	{
		m_ComponentArrays.insert(std::make_pair(typeName, std::make_unique<ComponentArray<T>>()));
	}
}