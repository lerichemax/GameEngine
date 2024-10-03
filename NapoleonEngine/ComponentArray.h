#pragma once
#include "Entity.h"
#include "Component.h"

#include <unordered_map>
#include <concepts>
#include <typeindex>

class IComponentArray
{
	friend class Coordinator;
	friend class ComponentManager;

public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
	virtual Component* const GetBaseData(Entity entity) = 0;
	virtual bool TryGetAllBaseData(Entity entity, std::vector<Component*>&) = 0;

protected:
	virtual void ForceInsertData(Component*, Entity entity) = 0;
};

template<typename T>
concept ComponentDerived = std::derived_from<T, Component>;

template<typename T>
class ComponentArray;

template<ComponentDerived T>
class ComponentArray<T> : public IComponentArray
{
	friend class ComponentManager;

public:
	ComponentArray() = default;
	~ComponentArray() = default;

	T* const InsertData(Entity entity);
	void RemoveData(Entity entity);
	Component* const GetBaseData(Entity entity) override;
	bool TryGetAllBaseData(Entity entity, std::vector<Component*>&) override;
	T* const GetData(Entity entity);
	std::vector<T*> GetAllData(Entity entity);
	void EntityDestroyed(Entity entity) override;

protected:
	void ForceInsertData(Component* pComp, Entity entity) override;

private:
	std::array<std::unique_ptr<T>, MAX_ENTITIES> m_Components{};
	std::unordered_multimap<Entity, size_t> m_EntityToIndex{};
	std::unordered_map<size_t, Entity> m_IndexToEntity{};
	size_t m_Size{};
};

#include "ComponentArray.inl"