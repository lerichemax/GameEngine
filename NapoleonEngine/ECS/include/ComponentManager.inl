#pragma once
#include "ComponentManager.h"

namespace ecs
{
	template<ComponentDerived T>
	void ComponentManager::RegisterComponent()
	{
		std::string type{ typeid(T).name() };

		if (m_ComponentTypes.find(type) == m_ComponentTypes.end())
		{
			m_ComponentTypes.insert(std::make_pair(type, m_NextComponentType++));
		}

		RegisterComponentArray<T>(type);
	}

	template<ComponentDerived T>
	ComponentType ComponentManager::GetComponentType()
	{
		RegisterComponent<T>();

		std::string type{ typeid(T).name() };

		return m_ComponentTypes[type];
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

	template<ComponentDerived T>
	std::vector<T*> const ComponentManager::FindComponentsOfType()
	{
		auto compArray = GetComponentArray<T>();

		return compArray->GetAllData();
	}


	template <ComponentDerived T>
	ComponentArray<T>* const ComponentManager::GetComponentArray()
	{
		std::string type{ typeid(T).name() };

		assert(m_ComponentTypes.find(type) != m_ComponentTypes.end() && "Component type not registered before use");

		return static_cast<ComponentArray<T>*>(m_ComponentArrays[type].get());
	}

	template<ComponentDerived T>
	void ComponentManager::RegisterComponentArray(std::string const& type)
	{
		if (m_ComponentArrays.find(type) == m_ComponentArrays.end())
		{
			m_ComponentArrays.insert(std::make_pair(type, std::make_unique<ComponentArray<T>>()));
		}
	}
}