#pragma once
#include "SceneObject.h"
#include "TextRendererComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextObject final : public SceneObject
	{
	public:
		explicit TextObject(const std::string& text, Font* const pFont);
		virtual ~TextObject() = default;
		TextObject(const TextObject & other) = delete;
		TextObject(TextObject && other) = delete;
		TextObject& operator=(const TextObject & other) = delete;
		TextObject& operator=(TextObject && other) = delete;
		
		void Update() override;

		void SetText(const std::string& text);
		
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		TextRendererComponent* m_pTxtRendererComponent;
	};
}
