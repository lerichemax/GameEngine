#pragma once
#include "Component.h"
#include "Shapes.h"

#include <memory>


class Texture2D;
struct RendererComponent : public Component
{
	friend class LayeredRendererSystem;
	friend class Renderer;
public:
	uint8_t Layer{};

	Texture2D* pTexture{ nullptr };

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void SetShape(geo::Shape* shape);

private:
	std::unique_ptr<geo::Shape> pShape{ nullptr };
};