#include "PCH.h"
#include "TransformSystem.h"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

void TransformSystem::Update()
{
	for (Entity const& entity : m_Entities)
	{
		auto* const pTrans = m_pRegistry->GetComponent<TransformComponent>(entity);

		if (!pTrans->IsActive())
		{
			continue;
		}

		RecursivelyUpdateHierarchy(pTrans);
	}
}

void TransformSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());

	m_pRegistry->SetSystemSignature<TransformSystem>(signature);
}

void TransformSystem::RecursivelyUpdateHierarchy(TransformComponent* const pTransformComponent) const
{
	if (pTransformComponent->m_pParent != nullptr)
	{
		RecursivelyUpdateHierarchy(pTransformComponent->m_pParent);

		pTransformComponent->m_LocalTransformMatrix = glm::inverse(pTransformComponent->m_pParent->m_WorldTransformMatrix) * pTransformComponent->m_WorldTransformMatrix;
	}
	else
	{
		pTransformComponent->m_LocalTransformMatrix = pTransformComponent->m_WorldTransformMatrix;
	}
}