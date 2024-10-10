#pragma once
#include "Component.h"
#include "ComponentManager.h"

struct CameraComponent : public Component
{
public:
	CameraComponent();

	unsigned int m_Width{};
	unsigned int m_Height{};

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};
};