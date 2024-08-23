#pragma once
#include "Entity.h"
#include "Serializer.h"

#include <set>

class ComponentManager;
class System : public IContextSerializable
{
	friend class Coordinator;
	friend class SystemManager;
	friend class Scene;
	friend class BaseScene;

public:
	virtual ~System() = default;

	virtual void SetSignature(Coordinator* const pRegistry) = 0;

protected:
	std::set<Entity> m_Entities;

	virtual void Initialize(ComponentManager* const pCompManager) {};
	virtual void Start(ComponentManager* const pCompManager) {};
	virtual void Update(ComponentManager* const pCompManager) {};

	void Serialize(StreamWriter& writer) const {};
	void Deserialize(JsonReader const* reader, SerializationMap& context) {}

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) {};

	virtual void AddEntity(Entity entity);

	System() = default;
	System(System const& other);
};