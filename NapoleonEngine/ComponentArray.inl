#pragma once
#include "ComponentArray.h"

template<ComponentDerived T>
T* const ComponentArray<T>::InsertData(Entity entity)
{
	auto pComponent = new T{};

	if (m_EntityToIndex.find(entity) != m_EntityToIndex.end() && static_cast<Component*>(pComponent)->IsUnique())
	{
		Debugger::Get().LogWarning(std::string{ "Component  is unique and can't be added twice to Entity " + std::to_string(entity) });
		delete pComponent;
		return nullptr;
	}

	size_t newIndex = m_Size;
	m_EntityToIndex.insert(std::make_pair(entity, newIndex));
	m_IndexToEntity[newIndex] = entity;
	m_Components[newIndex] = std::unique_ptr<T>(pComponent);
	m_Size++;
	return m_Components[newIndex].get();
}

template<ComponentDerived T>
void ComponentArray<T>::RemoveData(Entity entity)
{
	assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Removing a non existent component");

	auto range = m_EntityToIndex.equal_range(entity);

	for (auto it = range.first; it != range.second; it++)
	{
		size_t indexOfRemovedEntity = it->second;

		size_t indexOfLastElement = --m_Size;
		m_Components[indexOfRemovedEntity] = std::move(m_Components[indexOfLastElement]);

		Entity entityOfLastElement = m_IndexToEntity[indexOfLastElement];
		m_EntityToIndex.find(entityOfLastElement)->second = static_cast<Entity>(indexOfRemovedEntity);
		m_IndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

		m_IndexToEntity.erase(indexOfLastElement);
	}

	m_EntityToIndex.erase(entity);
}

template<ComponentDerived T>
Component* const ComponentArray<T>::GetBaseData(Entity entity)
{
	if (m_EntityToIndex.find(entity) == m_EntityToIndex.end())
	{
		Debugger::Get().LogWarning(std::string{ "Component  not found for entity " } + std::to_string(entity));
		return nullptr;
	}

	//assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Retrieving a non existent component");

	return m_Components[m_EntityToIndex.find(entity)->second].get();
}

template<ComponentDerived T>
bool ComponentArray<T>::TryGetAllBaseData(Entity entity, std::vector<Component*>& datas)
{
	if (m_EntityToIndex.find(entity) == m_EntityToIndex.end())
	{
		return false;
	}

	auto range = m_EntityToIndex.equal_range(entity);
	for (auto it = range.first; it != range.second; it++)
	{
		datas.push_back(m_Components[it->second].get());
	}
	return true;
}

template<ComponentDerived T>
T* const ComponentArray<T>::GetData(Entity entity)
{
	//assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Retrieving a non existent component");

	if (m_EntityToIndex.find(entity) == m_EntityToIndex.end())
	{
		//Debugger::Get().LogWarning("Component " + std::string(typeid(T).name()) + " not found for entity " + std::to_string(entity));
		return nullptr;
	}

	return m_Components[m_EntityToIndex.find(entity)->second].get();
}

template<ComponentDerived T>
std::vector<T*> ComponentArray<T>::GetAllData(Entity entity)
{
	std::vector<T*> toReturn;

	if (m_EntityToIndex.find(entity) == m_EntityToIndex.end())
	{
		return toReturn;
	}

	auto range = m_EntityToIndex.equal_range(entity);
	for (auto it = range.first; it != range.second; it++)
	{
		toReturn.push_back(m_Components[it->second].get());
	}
	return toReturn;
}


template<ComponentDerived T>
void ComponentArray<T>::EntityDestroyed(Entity entity)
{
	if (m_EntityToIndex.find(entity) != m_EntityToIndex.end())
	{
		RemoveData(entity);
	}
}

template<ComponentDerived T>
void ComponentArray<T>::ForceInsertData(Component* pComp, Entity entity)
{
	size_t newIndex = m_Size;
	m_EntityToIndex.insert(std::make_pair(entity, newIndex));
	m_IndexToEntity[newIndex] = entity;
	m_Components[newIndex] = std::unique_ptr<T>(static_cast<T*>(pComp));
	m_Size++;
}