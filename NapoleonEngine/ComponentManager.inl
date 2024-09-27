#pragma once
#include "ComponentManager.h"

template<ComponentDerived T>
void ComponentManager::RegisterComponent()
{
	size_t typeHash{ std::type_index(typeid(T)).hash_code() };

	if (m_ComponentTypes.find(typeHash) == m_ComponentTypes.end())
	{
		m_ComponentTypes.insert(std::make_pair(typeHash, m_NextComponentType++));

		Factory<Component, ComponentManager* const>::Get().RegisterType<T>([](ComponentManager* const compManager) {
			T* t = new T{};

			size_t compTypeHash{ std::type_index(typeid(T)).hash_code() };

			compManager->RegisterComponentArray<T>(compTypeHash);

			return t;
			});
	}

	RegisterComponentArray<T>(typeHash);
}

template<ComponentDerived T>
ComponentType ComponentManager::GetComponentType()
{
	RegisterComponent<T>();

	size_t typeHash{ std::type_index(typeid(T)).hash_code() };

	return m_ComponentTypes[typeHash];
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
	return GetComponentArray<T>()->GetData(entity);
}

template<ComponentDerived T>
std::vector<T*> ComponentManager::GetComponents(Entity entity)
{
	return GetComponentArray<T>()->GetAllData(entity);
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
	size_t typeHash{ std::type_index(typeid(T)).hash_code() };

	assert(m_ComponentTypes.find(typeHash) != m_ComponentTypes.end() && "Component type not registered before use");

	return static_cast<ComponentArray<T>*>(m_ComponentArrays[typeHash].get());
}

template<ComponentDerived T>
void ComponentManager::RegisterComponentArray(size_t typeHash)
{
	if (m_ComponentArrays.find(typeHash) == m_ComponentArrays.end())
	{
		m_ComponentArrays.insert(std::make_pair(typeHash, std::make_unique<ComponentArray<T>>()));
	}
}