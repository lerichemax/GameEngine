#include "PCH.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include <glm/gtx/matrix_transform_2d.hpp>
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
	UpdateTransform();
}

void TransformComponent::UpdateTransform()
{
	auto pParent = m_pGameObject->GetParent();
	if (pParent != nullptr)
	{
		auto parentWorld = pParent->GetTransform()->m_WorldPosition;
		m_WorldPosition = parentWorld + m_Position;
		m_WorldRotation = pParent->GetTransform()->m_WorldRotation + m_Rotation;
		m_WorldScale = pParent->GetTransform()->m_WorldScale * m_Scale;
	}
	else
	{
		m_WorldPosition = m_Position;
		m_WorldRotation = m_Rotation;
		m_WorldScale = m_Scale;
	}

	auto identityMatrix = glm::mat3(1.0f);
	
	glm::mat3x3 trans{};
	glm::mat3x3 rot{};
	glm::mat3x3 scale{};

	trans = glm::translate(identityMatrix, m_WorldPosition);
	
#ifdef GLM_FORCE_RADIANS
	rot = glm::rotate(identityMatrix, glm::radians(m_WorldRotation));
#else
	rot = glm::rotate(identityMatrix, m_WorldRotation);
#endif

	scale = glm::scale(identityMatrix, m_WorldScale);

	m_World = trans * rot * scale;
	
}

void TransformComponent::Translate(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;
	UpdateTransform();
}

void TransformComponent::Translate(glm::vec2 const& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
	UpdateTransform();
}

void TransformComponent::Scale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
	UpdateTransform();
}

void TransformComponent::Scale(glm::vec2 const& scale)
{
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;
	UpdateTransform();
}

void TransformComponent::Scale(float uniformScale)
{
	m_Scale.x = uniformScale;
	m_Scale.y = uniformScale;
	UpdateTransform();
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
	UpdateTransform();
}
