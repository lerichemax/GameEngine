#pragma once
#include "RendererComponent.h"
#include "Shapes.h"

#include <concepts>
#include <memory>

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
	std::shared_ptr<geo::Shape> m_pShape = nullptr;
};

template <ShapeDerived S>
void ShapeComponent::SetShape(S* const shape)
{
	ShapeFactory::GetInstance().RegisterType<S>([]() {
		return std::make_shared<S>();
		});

	m_pShape.reset(shape);
}