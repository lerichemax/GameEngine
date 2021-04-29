#include "PCH.h"
#include "SceneRenderer.h"
#include "RendererComponent.h"

using namespace empire;

void SceneRenderer::Render()
{
	for (auto group : m_pLayersGroup)
	{
		for (auto rendComp : group)
		{
			rendComp->Render(*rendComp->GetGameObject()->GetTransform());
		}
	}
}

void SceneRenderer::AddToGroup(RendererComponent* pRenderer, Layer layer)
{
	m_pLayersGroup[(int)layer].emplace_back(pRenderer);
	pRenderer->m_pSceneRenderer = this;
}

void SceneRenderer::RemoveFromGroup(RendererComponent* pRenderer, Layer layer)
{
	auto& currentGroup = m_pLayersGroup[(int)layer];
	currentGroup.erase(std::find(currentGroup.begin(), currentGroup.end(), pRenderer));
}