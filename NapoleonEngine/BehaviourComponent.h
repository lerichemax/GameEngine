#pragma once
#include "Component.h"

struct BehaviourComponent : public ECS_Component
{
public:
	virtual void Initialize() {};
	virtual void Update() {};

	void Serialize(StreamWriter& writer) const override { ECS_Component::Serialize(writer); };
	void Deserialize(JsonReader const* reader, SerializationMap& context) override { ECS_Component::Deserialize(reader, context); };

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override { ECS_Component::RestoreContext(reader, context); };
};
