#pragma once
#include "Component.h"
#include "Shapes.h"

#include <memory>


class Texture2D;
struct RendererComponent : public ecs::Component
{
	friend class LayeredRendererSystem;
	friend class Renderer;
public:
	RendererComponent();

	PROPERTY(uint8_t, Layer);

	PROPERTY(Texture2D*, pTexture);

	void SetShape(geo::Shape* shape);

private:
	PROPERTY(std::unique_ptr<geo::Shape>, pShape);
};

SERIALIZE_CLASS(RendererComponent, ecs::Component)