#include "PCH.h"
#include "ShapeComponent.h"

#include "Shapes.h"

ShapeComponent::ShapeComponent(geo::Shape* pShape)
	:m_pShape(pShape)
{
}

void ShapeComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(ShapeComponent).name());

	writer.StartObject("Shape");
	m_pShape->Serialize(writer);
	writer.EndObject();

	RendererComponent::Serialize(writer);
}

void ShapeComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	auto shapeObj = reader->ReadObject("Shape");

	if (reader != nullptr)
	{
		std::string type;
		shapeObj->ReadString("type", type);
		m_pShape = Factory<geo::Shape>::GetInstance().Create(type);
		if (m_pShape != nullptr)
		{
			m_pShape->Deserialize(shapeObj.get());
		}
	}

	RendererComponent::Deserialize(reader, context);
}

void ShapeComponent::Render()
{
	Renderer::GetInstance().RenderShape(*m_pShape);
}