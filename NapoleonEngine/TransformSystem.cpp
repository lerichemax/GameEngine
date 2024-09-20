#include "PCH.h"
#include "TransformSystem.h"

void TransformSystem::Update()
{
	for (Entity const& entity : m_Entities)
	{
		auto transComp = m_pRegistry->GetComponent<TransformComponent>(entity);

		if (!transComp->IsActive())
		{
			continue;
		}

		RecursivelyUpdateHierarchy(transComp);

		transComp->m_World = BuildTransformMatrix(transComp->m_WorldLocation, transComp->m_WorldRotation, transComp->m_WorldScale);
	}
}

void TransformSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());

	m_pRegistry->SetSystemSignature<TransformSystem>(signature);
}

void TransformSystem::RecursivelyUpdateHierarchy(TransformComponent* const transformComponent) const
{
	//if (!transformComponent->HasChanged())
	//{
	//	return;
	//}

	if (transformComponent->m_pParent != nullptr)
	{
		RecursivelyUpdateHierarchy(transformComponent->m_pParent);

		transformComponent->m_WorldLocation = transformComponent->m_pParent->m_WorldLocation + transformComponent->m_Location;
		transformComponent->m_WorldRotation = transformComponent->m_pParent->m_WorldRotation + transformComponent->m_Rotation;
		transformComponent->m_WorldScale = transformComponent->m_pParent->m_WorldScale * transformComponent->m_Scale;

		transformComponent->m_Location = transformComponent->m_WorldLocation - transformComponent->m_pParent->m_WorldLocation;
		transformComponent->m_Rotation = transformComponent->m_WorldRotation - transformComponent->m_pParent->m_WorldRotation;
		//transformComponent->m_Scale = transformComponent->m_pParent->m_WorldScale * transformComponent->m_WorldScale;
	}
	else
	{
		transformComponent->m_Location = transformComponent->m_WorldLocation;
		transformComponent->m_Rotation = transformComponent->m_WorldRotation;
		transformComponent->m_Scale = transformComponent->m_WorldScale;
	}

	transformComponent->m_OldPosition = transformComponent->m_WorldLocation;
	transformComponent->m_OldRotation = transformComponent->m_WorldRotation;
	transformComponent->m_OldScale = transformComponent->m_WorldScale;
}