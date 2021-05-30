#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "SceneRenderer.h"

namespace empire
{
	enum class Layer
	{
		preBacground,
		background,
		middleground,
		foreground,
		ui
	};
	
	class RendererComponent : public Component
	{
	public:
		RendererComponent(Layer layer = Layer::background);
		RendererComponent(std::string const& filename, Layer layer = Layer::background);
		RendererComponent(Texture2D* pText, Layer layer = Layer::background);

		RendererComponent* Clone() override { return new RendererComponent(*this); }

		RendererComponent(RendererComponent&& other) = delete;
		RendererComponent& operator=(RendererComponent const& rhs) = delete;
		RendererComponent& operator=(RendererComponent&& rhs) = delete;
		virtual ~RendererComponent();

		
		void Update() override{};
		virtual void Render(TransformComponent const& transform ) const;
		virtual void SetTexture(std::string const& filename);
		virtual void SetTexture(Texture2D* pText);

		float GetTextureWidth() const { return m_pTexture->GetWidth() * m_pGameObject->GetTransform()->GetScale().x; }
		float GetTextureHeight() const { return m_pTexture->GetHeight() * m_pGameObject->GetTransform()->GetScale().y; }

		void ChangeLayer(Layer newLayer);
	
	protected:
		friend void SceneRenderer::AddToGroup(RendererComponent* pRenderer, Layer layer);
		
		Texture2D* m_pTexture{};
		Layer m_Layer;
		SceneRenderer* m_pSceneRenderer;
		
		void Initialize() override;

	private:
		RendererComponent(RendererComponent const& other);
	};
}