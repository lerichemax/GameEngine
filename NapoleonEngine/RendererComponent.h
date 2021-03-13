#pragma once
#include "Component.h"
#include "Texture2D.h"
#include "TransformComponent.h"

namespace empire
{
	class RendererComponent : public Component
	{
	public:
		RendererComponent() = default;
		RendererComponent(std::string const& filename);
		RendererComponent(RendererComponent const& other) = delete;
		RendererComponent(RendererComponent&& other) = delete;
		RendererComponent& operator=(RendererComponent const& rhs) = delete;
		RendererComponent& operator=(RendererComponent&& rhs) = delete;
		virtual ~RendererComponent() = default;

		void Update() override{};
		void Render(TransformComponent const& transform = TransformComponent{}) const;
		void SetTexture(std::string const& filename);
	
	protected:
		Texture2D* m_pTexture{};
	};
}