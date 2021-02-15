#pragma once
#include "RendererComponent.h"
#include "Font.h"

#include <string>
#include <memory>

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
		~TextRendererComponent() = default;

		void Init(SceneObject* pParent) override;
		void Update() override;

		void SetText(const std::string& text);
	
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		Font* const m_pFont;
	};
}
