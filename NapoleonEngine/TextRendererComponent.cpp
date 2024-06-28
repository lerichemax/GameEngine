#include "PCH.h"
#include "TextRendererComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <SDL_ttf.h>

TextRendererComponent::TextRendererComponent(std::string const& text, Font* const pFont)
	:RendererComponent{Layer::uiGame},
	m_Text{text},
	m_NeedsUpdate{ true },
	m_pFont{pFont},
	m_TextColor{255,255,255}
{
}

TextRendererComponent::TextRendererComponent(TextRendererComponent const& other)
	:RendererComponent(other),
	m_Text(other.m_Text),
	m_NeedsUpdate(other.m_NeedsUpdate),
	m_pFont(other.m_pFont),
	m_TextColor(other.m_TextColor)
{
	
}

TextRendererComponent::~TextRendererComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void TextRendererComponent::Initialize()
{
	Update();
	RendererComponent::Initialize();
}

void TextRendererComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_TextColor);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto const texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		
		delete m_pTexture;
		m_pTexture = new Texture2D{ texture };
	}
}


void TextRendererComponent::Render(TransformComponent const& transform) const
{
	if (m_pTexture != nullptr)
	{
		const auto pos = transform.GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void TextRendererComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void TextRendererComponent::SetTextColor(Uint8 r, Uint8 g, Uint8 b)
{
	m_TextColor = SDL_Color{ r,g,b };
}

ECS_TextRendererComponent::ECS_TextRendererComponent(std::string const& text, std::shared_ptr<Font> font)
	:ECS_Component(true),
	m_Text{text},
	m_pFont{font}
{

}

void ECS_TextRendererComponent::SetTextColor(Uint8 r, Uint8 g, Uint8 b)
{
	m_TextColor = SDL_Color{ r,g,b };
}

void ECS_TextRendererComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(ECS_TextRendererComponent).name());
	writer.WriteString("txt", m_Text);
	Color color{ m_TextColor.r, m_TextColor.g , m_TextColor.b, m_TextColor.a };
	writer.StartObject("color");
	color.Serialize(writer);
	writer.EndObject();
	m_pFont->Serialize(writer);

	ECS_Component::Serialize(writer);
}

void ECS_TextRendererComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadString("txt", m_Text);
	auto colorObject = reader->ReadObject("color");
	Color color{ 0,0,0,0 };
	color.Deserialize(reader);
	m_TextColor = SDL_Color{ color.R, color.G, color.B, color.A };

	std::string fontName;
	reader->ReadString("fontName", fontName);

	int size;
	reader->ReadInt("fontSize", size);

	m_pFont = ResourceManager::GetInstance().GetFont(fontName, size);

	ECS_Component::Deserialize(reader, context);
}