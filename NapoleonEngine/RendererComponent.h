#pragma once
#include "Component.h"
#include "Shapes.h"

#include <memory>


template <typename T>
concept ShapeDerived = std::derived_from<T, geo::Shape>;

class Texture2D;
struct RendererComponent : public Component
{
	friend class LayeredRendererSystem;
public:
	uint8_t Layer{};

	Texture2D* pTexture{ nullptr };

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	template <ShapeDerived S> void SetShape(S* const shape);

private:
	geo::Shape* pShape = nullptr;
};

template <ShapeDerived S>
void RendererComponent::SetShape(S* const shape)
{
	Factory<geo::Shape>::GetInstance().RegisterType<S>([]() {
		return new S{};
		});

	pShape = shape;
}