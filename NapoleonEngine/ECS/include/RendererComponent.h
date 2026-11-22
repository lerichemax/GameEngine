#pragma once
#include "Component.h"
#include "Shapes.h"

#include <memory>

enum class RenderSpace
{
	World,
	Screen
};

class Texture2D;
struct RendererComponent : public ecs::Component
{
	friend class LayeredRendererSystem;
	friend class Renderer;
public:
	RendererComponent();

	MEMBER(uint8_t, Layer);

	MEMBER(Texture2D*, pTexture);
	MEMBER(RenderSpace, Space);

	void SetShape(std::unique_ptr<geo::Shape> pNewShape);

private:
	MEMBER(std::unique_ptr<geo::Shape>, pShape);
};

REGISTER_TYPE(RendererComponent, ecs::Component)