#pragma once
#include "Entity.h"

#include <unordered_map>

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
	virtual std::shared_ptr<IComponentArray> Clone() const = 0;

private:
	friend class Coordinator;
	virtual void CloneToNewEntity(std::shared_ptr<IComponentArray> other, Entity newEntity, Entity entityToClone) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void InsertData(Entity entity, T const& Component);
	void RemoveData(Entity entity);
	std::shared_ptr<T> GetData(Entity entity);
	void EntityDestroyed(Entity entity) override;

	~ComponentArray();

	std::shared_ptr<IComponentArray> Clone() const override { return std::make_shared<ComponentArray<T>>(ComponentArray<T>()); }

private:
	std::array<std::shared_ptr<T>, MAX_ENTITIES> m_Components{};
	std::unordered_map<Entity, size_t> m_EntityToIndex{};
	std::unordered_map<size_t, Entity> m_IndexToEntity{};
	size_t m_Size{};

	void CloneToNewEntity(std::shared_ptr<IComponentArray> other, Entity newEntity, Entity entityToClone) override;
};