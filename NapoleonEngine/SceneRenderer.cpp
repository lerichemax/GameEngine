#include "PCH.h"
#include "SceneRenderer.h"
#include "TextureRendererComponent.h"
#include "TextRendererComponent.h"

#include "ComponentManager.h"
#include "Coordinator.h"
#include "Texture2D.h"

#include <algorithm>
#include <SDL.h>
#include <SDL_ttf.h>

void SceneRenderer::Render()
{
	//std::for_each(m_pLayersGroup.begin(), m_pLayersGroup.end(),
	//	[](std::vector<RendererComponent*> const& group)
	//	{
	//		std::for_each(group.begin(), group.end(), [](RendererComponent* pRendComp)
	//			{
	//				if (pRendComp->GetGameObject()->IsActive() && pRendComp->IsEnable())
	//				{
	//					pRendComp->Render(*pRendComp->GetGameObject()->GetECSTransform());
	//				}
	//			});
	//	});
}

void SceneRenderer::AddToGroup(RendererComponent* pRenderer, Layer layer)
{
	m_pLayersGroup[static_cast<int>(layer)].emplace_back(pRenderer);
	//pRenderer->m_pSceneRenderer = this;
}

void SceneRenderer::RemoveFromGroup(RendererComponent* pRenderer, Layer layer)
{
	auto& currentGroup = m_pLayersGroup[static_cast<int>(layer)];
	currentGroup.erase(std::find(currentGroup.begin(), currentGroup.end(), pRenderer));
}

void LayeredRendererSystem::Update(ComponentManager* const pComponentManager)
{
	std::sort(m_EntityPerLayer.begin(), m_EntityPerLayer.end(), [&pComponentManager](Entity a, Entity b) {
		return pComponentManager->GetComponent<RendererComponent>(a)->m_Layer < pComponentManager->GetComponent<RendererComponent>(b)->m_Layer;
		});

	for (Entity const& entity : m_EntityPerLayer)
	{
		auto renderComp = pComponentManager->GetComponent<RendererComponent>(entity);

		if (!renderComp->IsActive())
		{
			continue;
		}
		renderComp->Render();
	}
}

void LayeredRendererSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());
	signature.set(pRegistry->GetComponentType<RendererComponent>());

	pRegistry->SetSystemSignature<LayeredRendererSystem>(signature);
}

void LayeredRendererSystem::AddEntity(Entity entity)
{
	System::AddEntity(entity);
	m_EntityPerLayer.push_back(entity);
}

void TextRendererSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto textRenderComp = pComponentManager->GetComponent<TextRendererComponent>(entity);
		auto renderComp = pComponentManager->GetComponent<TextureRendererComponent>(entity); //log if absent

		if (!textRenderComp->IsActive())
		{
			continue;
		}

		const auto surf = TTF_RenderText_Blended(textRenderComp->m_pFont->GetFont(), textRenderComp->m_Text.c_str(), textRenderComp->m_TextColor);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto const texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);


		if (renderComp->IsActive())
		{
			renderComp->m_pTexture.reset(new Texture2D{ texture });
		}
	}
}

void TextRendererSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());
	signature.set(pRegistry->GetComponentType<RendererComponent>());
	signature.set(pRegistry->GetComponentType<TextRendererComponent>());

	pRegistry->SetSystemSignature<TextRendererSystem>(signature);
}