#include "PCH.h"
#include "SceneRenderer.h"
#include "TextureRendererComponent.h"
#include "TextRendererComponent.h"

#include "ComponentManager.h"
#include "Coordinator.h"
#include "Texture2D.h"
#include "ResourceManager.h"

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
		return pComponentManager->GetComponent<TextureRendererComponent>(a)->m_Layer < pComponentManager->GetComponent<TextureRendererComponent>(b)->m_Layer;
		});

	for (Entity const& entity : m_EntityPerLayer)
	{
		auto renderComp = pComponentManager->GetComponent<TextureRendererComponent>(entity);

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
	signature.set(pRegistry->GetComponentType<TextureRendererComponent>());

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

		if (!textRenderComp->IsActive() || !textRenderComp-> m_NeedsUpdate)
		{
			continue;
		}

		auto renderComp = pComponentManager->GetComponent<TextureRendererComponent>(entity); //log if absent (also when adding)

		renderComp->m_pTexture = ResourceManager::GetInstance().GetTextTexture(textRenderComp->m_pFont->GetFont(), textRenderComp->m_Text.c_str(),
			textRenderComp->m_TextColor, textRenderComp->m_TextId);

		textRenderComp->m_NeedsUpdate = false;
	}
}

void TextRendererSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());
	signature.set(pRegistry->GetComponentType<TextureRendererComponent>());
	signature.set(pRegistry->GetComponentType<TextRendererComponent>());

	pRegistry->SetSystemSignature<TextRendererSystem>(signature);
}