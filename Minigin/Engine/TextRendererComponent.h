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
		TextRendererComponent(Font* const font);
		TextRendererComponent(TextRendererComponent const& other) = delete;
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent const& rhs) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& rhs) = delete;
		~TextRendererComponent() = default;
		void Update(std::string const& text);
	private:
		Font* const m_pFont;
		void MakeItPolymorphic() const override {} //temp do nothing
	};
}
