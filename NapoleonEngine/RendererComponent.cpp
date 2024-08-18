#include "PCH.h"
#include "RendererComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneRenderer.h"
#include "Scene.h"

void RendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteBool("renderer", true);
	writer.WriteInt("layer", m_Layer);
}

void RendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{

	int layer;
	reader->ReadInt("layer", layer);

	m_Layer = static_cast<uint8_t>(layer);
}