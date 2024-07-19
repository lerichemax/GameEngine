#pragma once
#include "Component.h"

class BehaviourComponent : public ECS_Component
{
	friend class BehaviourSystem;
public:
	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

protected:
	virtual void Initialize() {};
	virtual void Start() {};
	virtual void Update() {};
};
