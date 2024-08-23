#pragma once
#include "RendererComponent.h"
#include "Font.h"
#include "Event.h"

#include <string>
#include <SDL_pixels.h>

class TextRendererComponent : public Component
{
	friend class TextRendererSystem;
	friend class ResourceManager;

public:
	TextRendererComponent();
	TextRendererComponent(std::string const& text, Font* const font);

	~TextRendererComponent();

	void SetText(std::string const& text);
	void SetFont(Font* const pFont);
	void SetTextColor(Uint8 r, Uint8 g, Uint8 b);

	void Serialize(StreamWriter& writer) const;
	void Deserialize(JsonReader const* reader, SerializationMap& context);

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) {}

private:
	static int Id_Increment;
	int m_TextId;

	std::string m_Text;
	Font* m_pFont{};
	SDL_Color m_TextColor{ 255,255,255,255 };

	bool m_NeedsUpdate;

	static EventHandler<TextRendererComponent, int> OnAnyDestroyed;
};