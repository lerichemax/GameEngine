#pragma once
#include "RendererComponent.h"
#include "Shapes.h"

#include <concepts>
#include <memory>

template <typename T>
concept ShapeDerived = std::derived_from<T, geo::Shape>;

class ShapeComponent final : public RendererComponent
{
	friend class ShapeRenderer;

public:
	explicit ShapeComponent(geo::Shape* pShape);
	ShapeComponent() = default;


	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	template <ShapeDerived S> void SetShape(S* const shape);

protected:
	void Render() override;
	
private:
	geo::Shape* m_pShape = nullptr;
};

template <ShapeDerived S>
void ShapeComponent::SetShape(S* const shape)
{
	Factory<geo::Shape>::GetInstance().RegisterType<S>([]() {
		return new S{};
		});

	m_pShape = shape;
}