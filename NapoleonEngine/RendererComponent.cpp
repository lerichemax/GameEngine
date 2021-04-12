#include "PCH.h"
#include "RendererComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"


using namespace empire;

RendererComponent::RendererComponent(std::string const& filename)
	:m_pTexture(ResourceManager::GetInstance().GetTexture(filename))
{
}

RendererComponent::RendererComponent(Texture2D* pText)
	:m_pTexture(pText)
{
}

void RendererComponent::Render(TransformComponent const& transform) const
{
	if (m_pTexture != nullptr)
	{
		const auto pos = transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, GetTextureWidth(), GetTextureHeight());
	}
}

void RendererComponent::RenderNoScaling(TransformComponent const& transform) const
{
	if (m_pTexture != nullptr)
	{
		const auto pos = transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void RendererComponent::SetTexture(std::string const& filename)
{
	m_pTexture = ResourceManager::GetInstance().GetTexture(filename);
}

void RendererComponent::SetTexture(Texture2D* pText)
{
	m_pTexture = pText;
}