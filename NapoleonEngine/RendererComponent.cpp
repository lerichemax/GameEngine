#include "PCH.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

#include "Texture2D.h"

void RendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(RendererComponent)).hash_code()));

	writer.WriteInt("layer", Layer);

	if (IS_VALID(pTexture))
	{
		pTexture->Serialize(writer);
	}

	if (IS_VALID(pShape))
	{
		writer.StartObject("Shape");
		pShape->Serialize(writer);
		writer.EndObject();
	}

	Component::Serialize(writer);
}

void RendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	int layer;
	reader->ReadInt("layer", layer);

	Layer = static_cast<uint8_t>(layer);

	std::string filename;
	reader->ReadString("filepath", filename);
	if (!filename.empty())
	{
		pTexture = ResourceManager::Get().GetTexture(filename);
	}

	auto shapeObj = reader->ReadObject("Shape");

	if (shapeObj != nullptr)
	{
		int64_t type;
		shapeObj->ReadInt64("type", type);

		pShape = Factory<geo::Shape>::Get().Create(static_cast<size_t>(type));
		if (pShape != nullptr)
		{
			pShape->Deserialize(shapeObj.get());
		}
	}

	Component::Deserialize(reader, context);
}