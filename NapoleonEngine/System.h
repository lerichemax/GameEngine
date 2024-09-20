#pragma once
#include "Entity.h"
#include "Serializer.h"

#include <set>

class Coordinator;
class System : public IContextSerializable
{
	friend class Coordinator;
	friend class SystemManager;
	friend class Scene;
	friend class BaseScene;

public:
	virtual ~System() = default;

protected:
	Coordinator* m_pRegistry;
	std::set<Entity> m_Entities;

	virtual void Initialize() {};
	virtual void Start() {};
	virtual void Update() {};

	void Serialize(StreamWriter& writer) const {};
	void Deserialize(JsonReader const* reader, SerializationMap& context) {}

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) {};

	virtual void SetSignature() const = 0;

	virtual void AddEntity(Entity entity);

	System() = default;
	System(System const& other);
};