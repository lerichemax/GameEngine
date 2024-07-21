#pragma once
#include "Entity.h"
#include "Component.h"
#include "BehaviourComponent.h"

#include <unordered_map>
#include <concepts>

class IComponentArray
{
	friend class Coordinator;
	friend class ComponentManager;
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
	virtual std::shared_ptr<ECS_Component> GetBaseData(Entity entity) = 0;
	virtual bool TryGetBaseData(Entity entity, std::shared_ptr<ECS_Component>&) = 0;
	virtual bool TryGetAllBaseData(Entity entity, std::vector<std::shared_ptr<ECS_Component>>&) = 0;

protected:
	virtual void ForceInsertData(std::shared_ptr<ECS_Component>, Entity entity) = 0;
};

template<typename T>
concept ComponentDerived = std::derived_from<T, ECS_Component>;


template<typename T>
class ComponentArray;

template<ComponentDerived T>
class ComponentArray<T> : public IComponentArray
{
	friend class ComponentManager;

public:
	std::shared_ptr<T> InsertData(Entity entity, T const& Component);
	void RemoveData(Entity entity);
	std::shared_ptr<ECS_Component> GetBaseData(Entity entity) override;
	bool TryGetBaseData(Entity entity, std::shared_ptr<ECS_Component>& data) override;
	bool TryGetAllBaseData(Entity entity, std::vector<std::shared_ptr<ECS_Component>>&) override;
	std::shared_ptr<T> GetData(Entity entity);
	void EntityDestroyed(Entity entity) override;
	ComponentArray();

	~ComponentArray();

protected:
	void ForceInsertData(std::shared_ptr<ECS_Component> pComp, Entity entity) override;

private:
	std::array<std::shared_ptr<T>, MAX_ENTITIES> m_Components{};
	std::unordered_multimap<Entity, size_t> m_EntityToIndex{};
	std::unordered_map<size_t, Entity> m_IndexToEntity{};
	size_t m_Size{};
};

template<ComponentDerived T>
ComponentArray<T>::ComponentArray()
{
}

template<ComponentDerived T>
ComponentArray<T>::~ComponentArray()
{
}

template<ComponentDerived T>
std::shared_ptr<T> ComponentArray<T>::InsertData(Entity entity, T const& component)
{
	if (m_EntityToIndex.find(entity) != m_EntityToIndex.end() && static_cast<ECS_Component const&>(component).IsUnique())
	{
		Debugger::GetInstance().LogWarning("Component " + std::string(typeid(T).name()) + " is unique and can't be added twice to Entity " + std::to_string(entity));
		return nullptr;
	}

	size_t newIndex = m_Size;
	m_EntityToIndex.insert(std::make_pair(entity, newIndex));
	m_IndexToEntity[newIndex] = entity;
	m_Components[newIndex] = std::make_shared<T>(component);
	m_Size++;
	return m_Components[newIndex];
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
		m_Components[indexOfRemovedEntity] = m_Components[indexOfLastElement];

		Entity entityOfLastElement = m_IndexToEntity[indexOfLastElement];
		m_EntityToIndex.find(entityOfLastElement)->second = static_cast<Entity>(indexOfRemovedEntity);
		m_IndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

		m_IndexToEntity.erase(indexOfLastElement);
	}

	m_EntityToIndex.erase(entity);
}

template<ComponentDerived T>
std::shared_ptr<ECS_Component> ComponentArray<T>::GetBaseData(Entity entity)
{
	if (m_EntityToIndex.find(entity) == m_EntityToIndex.end())
	{
		Debugger::GetInstance().LogWarning("Component " + std::string(typeid(T).name()) + " not found for entity " + std::to_string(entity));
		return nullptr;
	}
	
	//assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Retrieving a non existent component");

	return std::static_pointer_cast<ECS_Component>(m_Components[m_EntityToIndex.find(entity)->second]);
}

template<ComponentDerived T>
bool ComponentArray<T>::TryGetBaseData(Entity entity, std::shared_ptr<ECS_Component>& data)
{
	if (m_EntityToIndex.find(entity) == m_EntityToIndex.end())
	{
		return false;
	}

	data = std::static_pointer_cast<ECS_Component>(m_Components[m_EntityToIndex.find(entity)->second]);
	return true;
}

template<ComponentDerived T>
bool ComponentArray<T>::TryGetAllBaseData(Entity entity, std::vector<std::shared_ptr<ECS_Component>>& datas)
{
	if (m_EntityToIndex.find(entity) == m_EntityToIndex.end())
	{
		return false;
	}

	auto range = m_EntityToIndex.equal_range(entity);
	for (auto it = range.first; it != range.second; it++)
	{
		datas.push_back(std::static_pointer_cast<ECS_Component>(m_Components[it->second]));
	}
	return true;
}

template<ComponentDerived T>
std::shared_ptr<T> ComponentArray<T>::GetData(Entity entity)
{
	//assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Retrieving a non existent component");

	if (m_EntityToIndex.find(entity) == m_EntityToIndex.end())
	{
		//Debugger::GetInstance().LogWarning("Component " + std::string(typeid(T).name()) + " not found for entity " + std::to_string(entity));
		return nullptr;
	}

	return m_Components[m_EntityToIndex.find(entity)->second];
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
void ComponentArray<T>::ForceInsertData(std::shared_ptr<ECS_Component> pComp, Entity entity)
{
	size_t newIndex = m_Size;
	m_EntityToIndex.insert(std::make_pair(entity, newIndex));
	m_IndexToEntity[newIndex] = entity;
	m_Components[newIndex] = std::static_pointer_cast<T>(pComp);
	m_Size++;
}