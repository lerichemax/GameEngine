#include "PCH.h"
#include "SceneRenderer.h"
#include "RendererComponent.h"
#include "ComponentManager.h"
#include "Coordinator.h"

#include <algorithm>

void SceneRenderer::Render()
{
	std::for_each(m_pLayersGroup.begin(), m_pLayersGroup.end(),
		[](std::vector<RendererComponent*> const& group)
		{
			std::for_each(group.begin(), group.end(), [](RendererComponent* pRendComp)
				{
					if (pRendComp->GetGameObject()->IsActive() && pRendComp->IsEnable())
					{
						pRendComp->Render(*pRendComp->GetGameObject()->GetTransform());
					}
				});
		});
}

void SceneRenderer::AddToGroup(RendererComponent* pRenderer, Layer layer)
{
	m_pLayersGroup[static_cast<int>(layer)].emplace_back(pRenderer);
	pRenderer->m_pSceneRenderer = this;
}

void SceneRenderer::RemoveFromGroup(RendererComponent* pRenderer, Layer layer)
{
	auto& currentGroup = m_pLayersGroup[static_cast<int>(layer)];
	currentGroup.erase(std::find(currentGroup.begin(), currentGroup.end(), pRenderer));
}

void LayeredRendererSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto renderComp = pComponentManager->GetComponent<ECS_RendererComponent>(entity);
		auto transComp = pComponentManager->GetComponent<ECS_TransformComponent>(entity);

		if (renderComp->m_pTexture != nullptr)
		{
			const auto pos = transComp->m_Position;
			Renderer::GetInstance().RenderTexture(*renderComp->m_pTexture, *transComp); //remove singleton ?
		}
	}
}