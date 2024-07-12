#pragma once
#include "Component.h"

class BehaviourComponent : public ECS_Component
{
public:
	virtual void Initialize() {};
	virtual void Update() {};

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;
};
