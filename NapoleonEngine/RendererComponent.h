#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "SceneRenderer.h"

#include <memory>

enum class Layer : uint8_t
{
	preBacground,
	background,
	middleground,
	foreground,
	uiGame,
	uiMenuBg,
	uiMenuMg,
	uiMenuFg
};
	
class RendererComponent : public Component
{
public:
	explicit RendererComponent(Layer layer = Layer::background);
	RendererComponent(std::string const& filename, Layer layer = Layer::background);
	RendererComponent(Texture2D* pText, Layer layer = Layer::background);

	RendererComponent(RendererComponent&& other) = delete;
	RendererComponent& operator=(RendererComponent const& rhs) = delete;
	RendererComponent& operator=(RendererComponent&& rhs) = delete;
	virtual ~RendererComponent();

	void Update() override{};
		
	virtual void SetTexture(std::string const& filename);
	virtual void SetTexture(Texture2D* pText);

	float GetTextureWidth() const;
	float GetTextureHeight() const;

	void ChangeLayer(Layer newLayer);
	
protected:
	friend class SceneRenderer;
		
	Texture2D* m_pTexture{};
	Layer m_Layer;
	SceneRenderer* m_pSceneRenderer;
		
	void Initialize() override;
	virtual void Render(TransformComponent const& transform) const;
		
	RendererComponent* Clone() const override { return new RendererComponent(*this); }
		
	RendererComponent(RendererComponent const& other);
		
};

struct ECS_RendererComponent : public ECS_Component
{
	std::shared_ptr<Texture2D> m_pTexture{};
	uint8_t m_Layer{};

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {}
};
