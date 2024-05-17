#include "PCH.h"
#include "ComponentArray.h"

#include "TransformComponent.h"
#include "RendererComponent.h"
#include "CameraComponent.h"
#include "ButtonComponent.h"
#include "TextRendererComponent.h"
#include "FPSCounter.h"

template class ComponentArray<ECS_TransformComponent>;
template class ComponentArray<ECS_RendererComponent>;
template class ComponentArray<ECS_CameraComponent>;
template class ComponentArray<ECS_ButtonComponent>;
template class ComponentArray<ECS_TextRendererComponent>;
template class ComponentArray<FPSCounterComponent>;

template<typename T>
ComponentArray<T>::~ComponentArray()
{

}

template<typename T>
void ComponentArray<T>::InsertData(Entity entity, T const& component)
{
	assert(m_EntityToIndex.find(entity) == m_EntityToIndex.end() && "Component added to the same entity more than once");

	size_t newIndex = m_Size;
	m_EntityToIndex[entity] = newIndex;
	m_IndexToEntity[newIndex] = entity;
	m_Components[newIndex] = std::make_shared<T>(component);
	m_Size++;
}

template<typename T>
void ComponentArray<T>::RemoveData(Entity entity)
{
	assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Removing a non existent component");

	size_t indexOfRemovedEntity = m_EntityToIndex[entity];
	size_t indexOfLastElement = --m_Size;
	m_Components[indexOfRemovedEntity].reset();
	m_Components[indexOfRemovedEntity] = m_Components[indexOfLastElement];
	m_Components[indexOfLastElement].reset();
	
	Entity entityOfLastElement = m_IndexToEntity[indexOfLastElement];
	m_EntityToIndex[entityOfLastElement] = static_cast<Entity>(indexOfRemovedEntity);
	m_IndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

	m_EntityToIndex.erase(entity);
	m_IndexToEntity.erase(indexOfLastElement);
}

template<typename T>
std::shared_ptr<T> ComponentArray<T>::GetData(Entity entity)
{
	assert(m_EntityToIndex.find(entity) != m_EntityToIndex.end() && "Retrieving a non existent component");

	return m_Components[m_EntityToIndex[entity]];
}


template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity)
{
	if (m_EntityToIndex.find(entity) != m_EntityToIndex.end())
	{
		RemoveData(entity);
	}
}

template<typename T>
void ComponentArray<T>::CloneToNewEntity(std::shared_ptr<IComponentArray> pOther, Entity newEntity, Entity entityToClone)
{
	if (m_EntityToIndex.find(newEntity) != m_EntityToIndex.end())
	{
		m_Components[m_EntityToIndex[newEntity]] = std::make_shared<T>(*std::static_pointer_cast<ComponentArray<T>>(pOther)->m_Components[entityToClone]);
	}
	else
	{
		size_t newIndex = m_Size;
		m_EntityToIndex[newEntity] = newIndex;
		m_IndexToEntity[newIndex] = newEntity;
		m_Components[newIndex] = std::make_shared<T>(*std::static_pointer_cast<ComponentArray<T>>(pOther)->m_Components[entityToClone]);
		m_Size++;
	}

}