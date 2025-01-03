#include "PCH.h"
#include "TransformSystem.h"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

void TransformSystem::Update()
{
	auto view = m_pRegistry->GetView<TransformComponent>();

	for (Entity entity : view)
	{
		auto* const pTrans = m_pRegistry->GetComponent<TransformComponent>(entity);

		if (!pTrans->IsActive())
		{
			continue;
		}

		RecursivelyUpdateHierarchy(pTrans);
	}
}

void TransformSystem::RecursivelyUpdateHierarchy(TransformComponent* const pTransformComponent) const
{
	if (pTransformComponent->m_pParent != nullptr)
	{
		RecursivelyUpdateHierarchy(pTransformComponent->m_pParent);

		pTransformComponent->m_WorldTransformMatrix = pTransformComponent->m_pParent->m_WorldTransformMatrix * pTransformComponent->m_LocalTransformMatrix;
	}
	else
	{
		pTransformComponent->m_LocalTransformMatrix = pTransformComponent->m_WorldTransformMatrix;
	}
}