#include "PCH.h"
#include "RendererComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneRenderer.h"
#include "Scene.h"

void RendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(RendererComponent).name());
	if (m_pTexture != nullptr)
	{
		m_pTexture->Serialize(writer);
	}

	writer.WriteInt("layer", m_Layer);
}

void RendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	std::string filename;
	reader->ReadString("filepath", filename);
	if (!filename.empty())
	{
		m_pTexture = ResourceManager::GetInstance().GetTexture(filename);
	}
	
	int layer;
	reader->ReadInt("layer", layer);

	m_Layer = static_cast<uint8_t>(layer);
}