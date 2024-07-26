#pragma once
#include "RendererComponent.h"
#include "Font.h"

#include <string>
#include <SDL_pixels.h>

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

struct ECS_TextRendererComponent : public ECS_Component
{
	ECS_TextRendererComponent() = default;
	ECS_TextRendererComponent(std::string const& text, std::shared_ptr<Font> font);

	std::string m_Text;
	std::shared_ptr<Font> m_pFont{};
	SDL_Color m_TextColor{ 255,255,255,255 };

	void SetTextColor(Uint8 r, Uint8 g, Uint8 b);

	void Serialize(StreamWriter& writer) const;
	void Deserialize(JsonReader const* reader, SerializationMap& context);

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) {}
};