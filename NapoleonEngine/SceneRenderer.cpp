#include "PCH.h"
#include "SceneRenderer.h"
#include "RendererComponent.h"

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