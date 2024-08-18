#include "PCH.h"
#include "TextureRendererComponent.h"

#include "Texture2D.h"
#include "ResourceManager.h"

void TextureRendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(TextureRendererComponent).name());
	if (m_pTexture != nullptr)
	{
		m_pTexture->Serialize(writer);
	}

	RendererComponent::Serialize(writer);
}

void TextureRendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	std::string filename;
	reader->ReadString("filepath", filename);
	if (!filename.empty())
	{
		m_pTexture = ResourceManager::GetInstance().GetTexture(filename);
	}

	RendererComponent::Deserialize(reader, context);
}

void TextureRendererComponent::Render()
{
	if (m_pTexture != nullptr)
	{
		auto transComp = GetGameObject()->GetComponent<ECS_TransformComponent>();
		Renderer::GetInstance().RenderTexture(*m_pTexture, *transComp); //remove singleton ?
	}
}