#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "TransformComponent.h"

namespace empire
{
	class RendererComponent : public Component
	{
	public:
		RendererComponent() = default;
		RendererComponent(std::string const& filename);
		RendererComponent(Texture2D* pText);
		RendererComponent(RendererComponent const& other) = delete;
		RendererComponent(RendererComponent&& other) = delete;
		RendererComponent& operator=(RendererComponent const& rhs) = delete;
		RendererComponent& operator=(RendererComponent&& rhs) = delete;
		virtual ~RendererComponent() = default;

		void Update() override{};
		void Render(TransformComponent const& transform = TransformComponent{}) const;
		void RenderNoScaling(TransformComponent const& transform) const;
		void SetTexture(std::string const& filename);
		void SetTexture(Texture2D* pText);

		float GetTextureWidth() const { return m_pTexture->GetWidth() * m_pParentObject->GetTransform()->GetScale().x; }
		float GetTextureHeight() const { return m_pTexture->GetHeight() * m_pParentObject->GetTransform()->GetScale().y; }
	protected:
		Texture2D* m_pTexture{};
	};
}