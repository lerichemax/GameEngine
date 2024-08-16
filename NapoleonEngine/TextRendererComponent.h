#pragma once
#include "RendererComponent.h"
#include "Font.h"

#include <string>
#include <SDL_pixels.h>

struct TextRendererComponent : public ECS_Component
{
	TextRendererComponent() = default;
	TextRendererComponent(std::string const& text, std::shared_ptr<Font> font);

	std::string m_Text;
	std::shared_ptr<Font> m_pFont{};
	SDL_Color m_TextColor{ 255,255,255,255 };

	void SetTextColor(Uint8 r, Uint8 g, Uint8 b);

	void Serialize(StreamWriter& writer) const;
	void Deserialize(JsonReader const* reader, SerializationMap& context);

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) {}
};