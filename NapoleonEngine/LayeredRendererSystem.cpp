#include "PCH.h"
#include "LayeredRendererSystem.h"

#include "RendererComponent.h"

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

void LayeredRendererSystem::SetSignature() const
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