#pragma once
#include "RendererComponent.h"

struct Shape;
class ShapeRenderer final : public RendererComponent
{
public:
	explicit ShapeRenderer(geo::Shape* pShape);
	ShapeRenderer* Clone() const override { return new ShapeRenderer(*this); }

	ShapeRenderer(ShapeRenderer&& other) = delete;
	ShapeRenderer& operator=(ShapeRenderer const& rhs) = delete;
	ShapeRenderer& operator=(ShapeRenderer&& rhs) = delete;
	~ShapeRenderer();

	//void Serialize(StreamWriter& writer) const override { writer.WriteString(" ", " "); };
	//void Deserialize(JsonReader const* reader, SerializationMap& context) override
	//{
	//	int t;
	//	reader->ReadInt(" ", t);
	//	context.Add(99999999);
	//};

	//void RestoreContext(JsonReader const* reader, SerializationMap const& context);

protected:
	void Render(TransformComponent const& transform) const override;
	
private:
	geo::Shape* m_pShape;

		
	ShapeRenderer(ShapeRenderer const& other);
};
