#include "PCH.h"
#include "TransformComponent.h"

#include "GameObject.h"

using namespace empire;

TransformComponent::TransformComponent()
	:m_Position(),
	m_WorldPosition(),
	m_Scale(1, 1),
	m_Rotation()
{
	
}


TransformComponent::TransformComponent(float x, float y)
	:m_Position{x,y},
	m_WorldPosition(),
	m_Scale(1, 1),
	m_Rotation()
{
}

void TransformComponent::Initialize()
{
	auto pParent = m_pGameObject->GetParent();
	if (pParent != nullptr)
	{
		auto parentWorld = pParent->GetTransform()->GetWorldPosition();
		m_WorldPosition = parentWorld + m_Position;
	}
}

void TransformComponent::Translate(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;
}


void TransformComponent::Translate(glm::vec2 const& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
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

	return *this;
}

void TransformComponent::Rotate(float rot)
{
	m_Rotation = rot;
}