#include "PCH.h"
#include "SceneRenderer.h"
#include "RendererComponent.h"
#include "TextRendererComponent.h"

#include "ComponentManager.h"
#include "Coordinator.h"
#include "Texture2D.h"
#include "ResourceManager.h"

#include <algorithm>
#include <SDL.h>
#include <SDL_ttf.h>

void LayeredRendererSystem::Update()
{
	std::sort(m_EntityPerLayer.begin(), m_EntityPerLayer.end(), [this](Entity a, Entity b) {
		return m_pRegistry->GetComponent<RendererComponent>(a)->Layer < m_pRegistry->GetComponent<RendererComponent>(b)->Layer;
		});

	for (Entity const& entity : m_EntityPerLayer)
	{
		auto renderComp = m_pRegistry->GetComponent<RendererComponent>(entity);

		if (!renderComp->IsActive())
		{
			continue;
		}
		
		if (renderComp->pTexture != nullptr)
		{
			auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
			Renderer::GetInstance().RenderTexture(*renderComp->pTexture, *pTransform); //remove singleton ?
		}

		if (renderComp->pShape != nullptr)
		{
			Renderer::GetInstance().RenderShape(*renderComp->pShape);
		}
	}
}

void LayeredRendererSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());
	signature.set(m_pRegistry->GetComponentType<RendererComponent>());

	m_pRegistry->SetSystemSignature<LayeredRendererSystem>(signature);
}

void LayeredRendererSystem::AddEntity(Entity entity)
{
	System::AddEntity(entity);
	m_EntityPerLayer.push_back(entity);
}

void TextRendererSystem::Update()
{
	for (Entity const& entity : m_Entities)
	{
		auto textRenderComp = m_pRegistry->GetComponent<TextRendererComponent>(entity);

		if (!textRenderComp->IsActive() || !textRenderComp->m_NeedsUpdate)
		{
			continue;
		}

		auto renderComp = m_pRegistry->GetComponent<RendererComponent>(entity); //log if absent (also when adding)

		renderComp->pTexture = ResourceManager::GetInstance().GetTextTexture(textRenderComp->m_pFont->GetFont(), textRenderComp->m_Text.c_str(),
			textRenderComp->m_TextColor, textRenderComp->m_TextId);

		textRenderComp->m_NeedsUpdate = false;
	}
}

void TextRendererSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());
	signature.set(m_pRegistry->GetComponentType<RendererComponent>());
	signature.set(m_pRegistry->GetComponentType<TextRendererComponent>());

	m_pRegistry->SetSystemSignature<TextRendererSystem>(signature);
}