#pragma once
#include "RendererComponent.h"
#include "Font.h"
#include "Event.h"

#include <string>
#include <SDL_pixels.h>

class TextRendererComponent : public ecs::Component
{
	friend class TextRendererSystem;
	friend class ResourceManager;

public:
	TextRendererComponent();
	TextRendererComponent(std::string const& text, std::string const& fullPath, int size);

	~TextRendererComponent();

	void SetText(std::string const& text);
	void SetFont(Font* const pFont);
	void SetTextColor(Uint8 r, Uint8 g, Uint8 b);
	SDL_Color GetSDLColor() const;

private:
	static int Id_Increment;
	int m_TextId;

	PROPERTY(std::string, m_Text);
	PROPERTY(Font*, m_pFont);
	PROPERTY(Color*, m_pTextColor);

	bool m_NeedsUpdate{true};

	static EventHandler<TextRendererComponent, int> OnAnyDestroyed;
};

SERIALIZE_CLASS(TextRendererComponent, ecs::Component)