#pragma once
#include "Component.h"

#include <memory>

class Texture2D;
class RendererComponent : public Component
{
	friend class LayeredRendererSystem;
public:
	uint8_t m_Layer{};

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

protected:
	virtual void Render() {};

};
