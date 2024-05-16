#pragma once
#include "Component.h"
#include "ComponentArray.h"
#include "Entity.h"

#include <unordered_map>
#include <memory>

class ComponentManager
{
public:

	template<typename T> ComponentType GetComponentType();
	template<typename T> void AddComponent(Entity entity, T const& component);
	template<typename T> void RemoveComponent(Entity entity);
	template<typename T> std::shared_ptr<T> GetComponent(Entity entity);
	
	void EntityDestroyed(Entity entity);

private:
	friend class Coordinator;

	static std::unordered_map<const char*, ComponentType> m_ComponentTypes;
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays;
	static ComponentType m_NextComponentType;

	template<typename T> void RegisterComponent();
	template <typename T> std::shared_ptr<ComponentArray<T>> GetComponentArray();
	template <typename T> void CheckComponentRegistered();
};

template<typename T> 
void ComponentManager::RegisterComponent()
{
	const char* typeName = typeid(T).name();

	m_ComponentTypes.insert(std::make_pair(typeName, m_NextComponentType++));
}

template<typename T>
ComponentType ComponentManager::GetComponentType()
{
	CheckComponentRegistered<T>();

	const char* typeName = typeid(T).name();

	return m_ComponentTypes[typeName];
}

template<typename T> 
void ComponentManager::AddComponent(Entity entity, T const& component)
{
	CheckComponentRegistered<T>();

	GetComponentArray<T>()->InsertData(entity, component);
}

template<typename T> 
void ComponentManager::RemoveComponent(Entity entity)
{
	return GetComponentArray<T>()->RemoveData(entity);
}

template<typename T> 
std::shared_ptr<T> ComponentManager::GetComponent(Entity entity)
{
	return GetComponentArray<T>()->GetData(entity);
}


template <typename T> 
std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray()
{
	const char* typeName = typeid(T).name();

	assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component type not registered before use");

	return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
}

template <typename T> 
void ComponentManager::CheckComponentRegistered()
{
	const char* typeName = typeid(T).name();

	if (m_ComponentTypes.find(typeName) == m_ComponentTypes.end())
	{
		RegisterComponent<T>();
	}
	if (m_ComponentArrays.find(typeName) == m_ComponentArrays.end())
	{
		m_ComponentArrays.insert(std::make_pair(typeName, std::make_shared<ComponentArray<T>>()));
	}
}