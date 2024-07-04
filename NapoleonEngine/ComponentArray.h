#pragma once
#include "Entity.h"

#include "Component.h"

#include <unordered_map>
#include <concepts>

struct ECS_Component;
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
	virtual std::shared_ptr<IComponentArray> Clone() const = 0;
	virtual std::shared_ptr<ECS_Component> GetBaseData(Entity entity) = 0;

protected:
	virtual void CloneToNewEntity(std::shared_ptr<IComponentArray> other, Entity newEntity, Entity entityToClone) = 0;
	virtual void ForceInsertData(std::shared_ptr<ECS_Component>, Entity entity) = 0;

private:
	friend class Coordinator;
	friend class ComponentManager;
};

template<typename T>
concept ComponentDerived = std::derived_from<T, ECS_Component>;

template<typename T>
class ComponentArray;

template<ComponentDerived T>
class ComponentArray<T> : public IComponentArray
{
public:
	std::shared_ptr<T> InsertData(Entity entity, T const& Component);
	void RemoveData(Entity entity);
	std::shared_ptr<ECS_Component> GetBaseData(Entity entity) override;
	std::shared_ptr<T> GetData(Entity entity);
	void EntityDestroyed(Entity entity) override;
	ComponentArray();

	~ComponentArray();

	std::shared_ptr<IComponentArray> Clone() const override { return std::make_shared<ComponentArray<T>>(ComponentArray<T>()); }

protected:
	void CloneToNewEntity(std::shared_ptr<IComponentArray> other, Entity newEntity, Entity entityToClone) override;
	void ForceInsertData(std::shared_ptr<ECS_Component> pComp, Entity entity) override;

private:
	std::array<std::shared_ptr<T>, MAX_ENTITIES> m_Components{};
	std::unordered_map<Entity, size_t> m_EntityToIndex{};
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
	//TODO: check for unique component
	//assert(m_EntityToIndex.find(entity) == m_EntityToIndex.end() && "Component added to the same entity more than once");

	size_t newIndex = m_Size;
	m_EntityToIndex[entity] = newIndex;
	m_IndexToEntity[newIndex] = entity;
	m_Components[newIndex] = std::make_shared<T>(component);
	m_Size++;
	return m_Components[newIndex];
}

template<ComponentDerived T>
void ComponentArray<T>::RemoveData(Entity entity)
{
	assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Removing a non existent component");

	size_t indexOfRemovedEntity = m_EntityToIndex[entity];
	size_t indexOfLastElement = --m_Size;
	m_Components[indexOfRemovedEntity] = m_Components[indexOfLastElement];

	Entity entityOfLastElement = m_IndexToEntity[indexOfLastElement];
	m_EntityToIndex[entityOfLastElement] = static_cast<Entity>(indexOfRemovedEntity);
	m_IndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

	m_EntityToIndex.erase(entity);
	m_IndexToEntity.erase(indexOfLastElement);
}

template<ComponentDerived T>
std::shared_ptr<ECS_Component> ComponentArray<T>::GetBaseData(Entity entity)
{
	assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Retrieving a non existent component");

	return std::static_pointer_cast<ECS_Component>(m_Components[m_EntityToIndex[entity]]);
}

template<ComponentDerived T>
std::shared_ptr<T> ComponentArray<T>::GetData(Entity entity)
{
	assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Retrieving a non existent component");

	return m_Components[m_EntityToIndex[entity]];
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
void ComponentArray<T>::CloneToNewEntity(std::shared_ptr<IComponentArray> pOther, Entity newEntity, Entity entityToClone)
{
	auto otherCompArray = std::static_pointer_cast<ComponentArray<T>>(pOther);

	if (m_EntityToIndex.find(newEntity) != m_EntityToIndex.end())
	{
		m_Components[m_EntityToIndex[newEntity]] = std::make_shared<T>(*std::static_pointer_cast<ComponentArray<T>>(pOther)->m_Components[otherCompArray->m_EntityToIndex[entityToClone]]);
	}
	else
	{
		size_t newIndex = m_Size;
		m_EntityToIndex[newEntity] = newIndex;
		m_IndexToEntity[newIndex] = newEntity;
		m_Components[newIndex] = std::make_shared<T>(*otherCompArray->m_Components[otherCompArray->m_EntityToIndex[entityToClone]]);
		m_Size++;
	}
}

template<ComponentDerived T>
void ComponentArray<T>::ForceInsertData(std::shared_ptr<ECS_Component> pComp, Entity entity)
{
	size_t newIndex = m_Size;
	m_EntityToIndex[entity] = newIndex;
	m_IndexToEntity[newIndex] = entity;
	m_Components[newIndex] = std::static_pointer_cast<T>(pComp);
	m_Size++;
}