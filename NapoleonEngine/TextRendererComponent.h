#pragma once
#include "RendererComponent.h"
#include "Font.h"

#include <string>
#include <SDL_pixels.h>

namespace empire
{
	class TextRendererComponent final : public RendererComponent
	{
	public:
		TextRendererComponent(std::string const& text, Font* const pFont);

		TextRendererComponent* Clone() override { return new TextRendererComponent(*this); }
		
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent const& rhs) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& rhs) = delete;
		~TextRendererComponent();
		
		void Update() override;
		void Render(TransformComponent const& transform) const override;
		void SetText(const std::string& text);
		void SetTextColor(Uint8 r, Uint8 g, Uint8 b);
	protected:
		std::string m_Text;
		bool m_NeedsUpdate;
	
	private:
		Font* const m_pFont;
		SDL_Color m_TextColor;

		void Initialize() override;

		TextRendererComponent(TextRendererComponent const& other);
	};
}
