#include "PCH.h"
#include "ShapeRenderer.h"
#include "ShapeComponent.h"

void ShapeRenderer::Update()
{
	for (Entity const& entity : m_Entities)
	{
		auto shape = m_pCompManager->GetComponent<ShapeComponent>(entity);

		if (shape != nullptr && shape->IsActive())
		{
			Renderer::GetInstance().RenderShape(*shape->m_pShape); // remove singleton call ?
		}
	}
}

void ShapeRenderer::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());
	signature.set(pRegistry->GetComponentType<ShapeComponent>());

	pRegistry->SetSystemSignature<ShapeRenderer>(signature);
}