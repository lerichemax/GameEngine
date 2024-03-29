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
		explicit TextRendererComponent(std::string const& text, Font* const pFont);

		TextRendererComponent* Clone() const override { return new TextRendererComponent(*this); }
		
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent const& rhs) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& rhs) = delete;
		~TextRendererComponent();
		
		void Update() override;
		
		void SetText(const std::string& text);
		void SetTextColor(Uint8 r, Uint8 g, Uint8 b);
	
	protected:
		std::string m_Text;
		bool m_NeedsUpdate;

		void Render(TransformComponent const& transform) const override;
	
	private:
		Font* const m_pFont;
		SDL_Color m_TextColor;

		void Initialize() override;

		TextRendererComponent(TextRendererComponent const& other);
		
		void SetTexture(std::string const& ) override {};
		void SetTexture(Texture2D* ) override{}
	};
}
