#pragma once
#include "Component.h"
#include "Shapes.h"

#include <concepts>
#include <memory>

class ShapeComponent final : public ECS_Component // add layers ?
{
	friend class ShapeRenderer;

public:
	explicit ShapeComponent(geo::Shape* pShape);
	ShapeComponent() = default;

	ShapeComponent(ShapeComponent const& other) = delete;
	ShapeComponent(ShapeComponent&& other) = delete;
	ShapeComponent& operator=(ShapeComponent const& rhs) = delete;
	ShapeComponent& operator=(ShapeComponent&& rhs) = delete;

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	template <ShapeDerived S> void SetShape(S* const shape); //unsafe
	
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