#include "PCH.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

#include "Texture2D.h"

void RendererComponent::SetShape(geo::Shape* shape)
{
	pShape = std::unique_ptr<geo::Shape>(std::forward<geo::Shape*>(shape));
} 

void RendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt("layer", Layer);

	if (IS_VALID(pTexture))
	{
		writer.WriteObject("texture", pTexture);
	}

	if (IS_VALID(pShape))
	{
		writer.WriteObject("Shape", pShape.get());
	}

	Component::Serialize(writer);
}

void RendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	int layer;
	reader->ReadInt("layer", layer);

	Layer = static_cast<uint8_t>(layer);

	auto pTextureReader = reader->ReadObject("texture");
	if (IS_VALID(pTextureReader))
	{
		std::string filename;
		pTextureReader->ReadString("filepath", filename);
		if (!filename.empty())
		{
			pTexture = ResourceManager::Get().GetTexture(filename);
		}
	}

	auto shapeObj = reader->ReadObject("Shape");

	if (IS_VALID(shapeObj)) //have this handled by shape deserializer ?
	{
		std::string type;
		shapeObj->ReadString("type", type);

		pShape = std::unique_ptr<geo::Shape>(std::forward<geo::Shape*>(Factory<geo::Shape>::Get().Create(type)));
		if (pShape != nullptr)
		{
			pShape->Deserialize(shapeObj.get());
		}
	}

	Component::Deserialize(reader, context);
}