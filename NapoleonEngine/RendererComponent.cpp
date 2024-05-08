#include "PCH.h"
#include "RendererComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneRenderer.h"
#include "Scene.h"

RendererComponent::RendererComponent(Layer layer)
	:m_pTexture(nullptr),
	m_Layer(layer),
	m_pSceneRenderer(nullptr)
{
	
}

RendererComponent::RendererComponent(std::string const& filename, Layer layer)
	:m_pTexture(/*ResourceManager::GetInstance().GetTexture(filename)*/),
	m_Layer(layer),
	m_pSceneRenderer(nullptr)
{
	printf(filename.c_str());
}

RendererComponent::RendererComponent(Texture2D* pText, Layer layer)
	:m_pTexture(pText),
	m_Layer(layer),
	m_pSceneRenderer(nullptr)
{
}

RendererComponent::RendererComponent(RendererComponent const& other)
	:m_pTexture(other.m_pTexture), //shallow copy okay since the resource is managed by ResourceManager
	m_Layer(other.m_Layer),
	m_pSceneRenderer(other.m_pSceneRenderer)
{
}

RendererComponent::~RendererComponent()
{
	if (m_pSceneRenderer != nullptr)
	{
		m_pSceneRenderer->RemoveFromGroup(this, m_Layer);
		m_pSceneRenderer = nullptr;
	}
}

void RendererComponent::Render(TransformComponent const& transform) const
{
	if (m_pTexture != nullptr)
	{
		const auto pos = transform.GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, transform);
	}
}

void RendererComponent::SetTexture(std::string const& filename)
{
	printf(filename.c_str());
	//m_pTexture = ResourceManager::GetInstance().GetTexture(filename);
}

void RendererComponent::SetTexture(Texture2D* pText)
{
	m_pTexture = pText;
}

float RendererComponent::GetTextureWidth() const
{
	if (m_pTexture == nullptr)
	{
		Debugger::GetInstance().LogWarning("RendererComponent::GetTextureWidth - > texture is nullptr, returning 0");
		return 0.f;
	}
	return m_pTexture->GetWidth() * m_pGameObject->GetECSTransform()->GetScale().x;
}

float RendererComponent::GetTextureHeight() const
{
	if (m_pTexture == nullptr)
	{
		Debugger::GetInstance().LogWarning("RendererComponent::GetTextureHeight - > texture is nullptr, returning 0");
		return 0.f;
	}
	return m_pTexture->GetHeight() * m_pGameObject->GetECSTransform()->GetScale().y;
}

void RendererComponent::Initialize()
{
	m_pGameObject->GetParentScene()->AddToGroup(this, m_Layer);
}

void RendererComponent::ChangeLayer(Layer newLayer)
{
	if (m_pGameObject != nullptr)
	{
		m_pGameObject->GetParentScene()->RemoveFromGroup(this, m_Layer);
		m_pGameObject->GetParentScene()->AddToGroup(this, newLayer);
	}

	m_Layer = newLayer;
}
