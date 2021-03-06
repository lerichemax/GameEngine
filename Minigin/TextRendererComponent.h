#pragma once
#include "RendererComponent.h"
#include "Font.h"

#include <string>
#include <SDL_pixels.h>

namespace dae
{
	class TextRendererComponent final : public RendererComponent
	{
	public:
		TextRendererComponent(std::string const& text, Font* const pFont);
		TextRendererComponent(TextRendererComponent const& other) = delete;
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent const& rhs) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& rhs) = delete;
		~TextRendererComponent();

		void Init(SceneObject* pParent) override;
		void Update() override;

		void SetText(const std::string& text);
		void SetTextColor(Uint8 r, Uint8 g, Uint8 b);
	protected:
		std::string m_Text;
		bool m_NeedsUpdate;
	private:
		Font* const m_pFont;
		SDL_Color m_TextColor;
	};
}
