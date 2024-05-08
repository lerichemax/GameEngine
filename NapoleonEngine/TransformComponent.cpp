#include "PCH.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include <glm/gtx/matrix_transform_2d.hpp>

TransformComponent::TransformComponent()
	:m_Position(),
	m_Scale(1, 1),
	m_Rotation(),
	m_World(),
	m_WorldPosition(),
	m_WorldScale(),
	m_WorldRotation()
{
	
}


TransformComponent::TransformComponent(float x, float y)
	:m_Position{x,y},
	m_Scale(1, 1),
	m_Rotation(),
	m_World(),
	m_WorldPosition(),
	m_WorldScale(),
	m_WorldRotation()
	
{
}

void TransformComponent::Initialize()
{
	Update();
}

void TransformComponent::Update()
{
	auto* pParent = m_pGameObject->GetParent();
	if (pParent != nullptr)
	{
		/*auto const parentWorld = pParent->GetECSTransform()->m_WorldPosition;
		m_WorldPosition = parentWorld + m_Position;
		m_WorldRotation = pParent->GetECSTransform()->m_WorldRotation + m_Rotation;
		m_WorldScale = pParent->GetECSTransform()->m_WorldScale * m_Scale;*/
	}
	else
	{
		m_WorldPosition = m_Position;
		m_WorldRotation = m_Rotation;
		m_WorldScale = m_Scale;
	}

	m_World = BuildTransformMatrix(m_WorldPosition, m_WorldRotation, m_WorldScale);
}

void TransformComponent::Translate(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void TransformComponent::Translate(glm::vec2 const& pos)
{
	m_Position = pos;
}

void TransformComponent::SetWorldPosition(float x, float y)
{
	SetWorldPosition(glm::vec2{ x,y });
}

void TransformComponent::SetWorldPosition(glm::vec2 const& worldPos)
{
	auto* const parentObj = m_pGameObject->GetParent();
	if (parentObj == nullptr)
	{
		Translate(worldPos);
	}
	else
	{
		auto const parentWorld = parentObj->GetECSTransform()->GetWorldPosition();

		auto const pos = worldPos - parentWorld;

		Translate(pos);
	}
}

void TransformComponent::Scale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}

void TransformComponent::Scale(glm::vec2 const& scale)
{
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;
}

void TransformComponent::Scale(float uniformScale)
{
	m_Scale.x = uniformScale;
	m_Scale.y = uniformScale;
}

TransformComponent& TransformComponent::operator=(TransformComponent const& rhs)
{
	m_Position.x = rhs.m_Position.x;
	m_Position.y = rhs.m_Position.y;

	m_WorldPosition.x = rhs.m_WorldPosition.x;
	m_WorldPosition.y = rhs.m_WorldPosition.y;

	m_Scale.x = rhs.m_Scale.x;
	m_Scale.y = rhs.m_Scale.y;

	m_Rotation = rhs.m_Rotation;

	m_WorldRotation = rhs.m_WorldRotation;
	m_WorldScale = rhs.m_WorldScale;

	m_World = rhs.m_World;
	
	return *this;
}

void TransformComponent::Rotate(float rot)
{
	m_Rotation = rot;
}

void ECS_TransformComponent::Translate(vec2 const& translation)
{
	m_Position = translation;
}

void ECS_TransformComponent::Translate(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void ECS_TransformComponent::Scale(vec2 const& scale)
{
	m_Scale = {scale.x, scale.y};
}

void ECS_TransformComponent::Scale(float scale)
{
	m_Scale = { scale, scale };
}

void ECS_TransformComponent::Rotate(float rotation)
{
	m_Rotation = rotation;
}

TransformSystem::TransformSystem(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());

	pRegistry->SetSystemSignature<TransformSystem>(signature);
}

void TransformSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto transComp = pComponentManager->GetComponent<ECS_TransformComponent>(entity);

		transComp->m_WorldPosition = transComp->m_Position;
		transComp->m_WorldRotation = transComp->m_Rotation;
		transComp->m_WorldScale = transComp->m_Scale;

		transComp->m_World = BuildTransformMatrix(transComp->m_WorldPosition, transComp->m_WorldRotation, transComp->m_WorldScale);
	}
}