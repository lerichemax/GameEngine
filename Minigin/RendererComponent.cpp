#include "MiniginPCH.h"
#include "RendererComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"


using namespace dae;

RendererComponent::RendererComponent(std::string const& filename)
	:m_pTexture(ResourceManager::GetInstance().LoadTexture(filename))
{
}

void RendererComponent::Render(TransformComponent const& transform) const
{
	if (m_pTexture != nullptr)
	{
		const auto pos = transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void RendererComponent::SetTexture(std::string const& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
