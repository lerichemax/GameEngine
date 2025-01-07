#include "PCH.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "lua.hpp"

TransformComponent::TransformComponent() 
	: Component(true),
	m_WorldTransformMatrix{ glm::mat3(1.0f) },
	m_LocalTransformMatrix{ glm::mat3(1.0f) }
{}

void TransformComponent::SetLocalLocation(vec2 const& loc)
{
	vec3 pos3{ loc.x, loc.y, 1 };

	m_LocalTransformMatrix[2] = pos3;

	if (m_pParent != nullptr)
	{
		m_WorldTransformMatrix = m_LocalTransformMatrix * m_pParent->m_WorldTransformMatrix;
	}
	else
	{
		m_WorldTransformMatrix = m_LocalTransformMatrix;
	}

	m_bWasChanged = true;
}

void TransformComponent::SetLocalLocation(float x, float y)
{
	SetLocalLocation({ x,y });
}

void TransformComponent::SetLocation(vec2 const& loc)
{
	vec3 pos3{ loc.x, loc.y, 1 };

	m_WorldTransformMatrix[2] = pos3;

	if (m_pParent != nullptr)
	{
		m_LocalTransformMatrix = glm::inverse(m_pParent->m_WorldTransformMatrix) * m_WorldTransformMatrix;
	}
	else
	{
		m_LocalTransformMatrix = m_WorldTransformMatrix;
	}

	m_bWasChanged = true;
}

void TransformComponent::SetLocation(float x, float y)
{
	SetLocation(vec2{ x,y });
}

void TransformComponent::Scale(vec2 const& scale)
{
	vec3 pos3{ scale.x, scale.y, 1 };

	mat3x3 scaling = glm::scale(mat3x3{1.0f}, scale);

	m_WorldTransformMatrix = m_WorldTransformMatrix * scaling;

	if (m_pParent != nullptr)
	{

		m_LocalTransformMatrix = glm::inverse(m_pParent->m_WorldTransformMatrix) * m_WorldTransformMatrix;
	}
	else
	{
		m_LocalTransformMatrix = m_WorldTransformMatrix;
	}

	m_bWasChanged = true;
}

void TransformComponent::Scale(float scale)
{
	Scale({ scale, scale });
}

void TransformComponent::Rotate(float rotation)
{
	mat3x3 rotationMat = glm::rotate(mat3x3{ 1.0f }, rotation);

	m_WorldTransformMatrix = m_WorldTransformMatrix * rotationMat;

	if (m_pParent != nullptr)
	{
		m_LocalTransformMatrix = glm::inverse(m_pParent->m_WorldTransformMatrix) * m_WorldTransformMatrix;
	} 
	else
	{
		m_LocalTransformMatrix = m_WorldTransformMatrix;
	}

	m_bWasChanged = true;
}

glm::vec2 TransformComponent::GetLocation() 
{ 
	UpdateTransformHierarchy();
	return m_WorldTransformMatrix[2]; 
}
glm::vec2 TransformComponent::GetLocalLocation() 
{ 
	UpdateTransformHierarchy();
	return m_LocalTransformMatrix[2];
}
glm::vec2 TransformComponent::GetScale() 
{ 
	UpdateTransformHierarchy();
	vec2 scale{};
	scale.x = glm::sqrt(glm::pow(m_WorldTransformMatrix[0][0], 2.f) + glm::pow(m_WorldTransformMatrix[0][1], 2.f));
	scale.y = glm::sqrt(glm::pow(m_WorldTransformMatrix[1][0], 2.f) + glm::pow(m_WorldTransformMatrix[1][1], 2.f));

	return scale;
}
glm::vec2 TransformComponent::GetLocalScale() 
{ 
	UpdateTransformHierarchy();
	vec2 scale{};
	scale.x = glm::sqrt(glm::pow(m_LocalTransformMatrix[0][0], 2.f) + glm::pow(m_LocalTransformMatrix[0][1], 2.f));
	scale.y = glm::sqrt(glm::pow(m_LocalTransformMatrix[1][0], 2.f) + glm::pow(m_LocalTransformMatrix[1][1], 2.f));

	return scale;
}
float TransformComponent::GetRotation() 
{ 
	UpdateTransformHierarchy();
	float angle = glm::atan(m_WorldTransformMatrix[0][1], m_WorldTransformMatrix[0][0]);
	return angle;
}
float TransformComponent::GetLocalRotation() 
{
	UpdateTransformHierarchy();
	float angle = glm::atan(m_LocalTransformMatrix[0][1], m_LocalTransformMatrix[0][0]);
	return angle;
}

glm::mat3x3 const& TransformComponent::GetWorldTransformMatrix()
{
	UpdateTransformHierarchy();
	return m_WorldTransformMatrix;
}

glm::mat3x3 TransformComponent::BuildTransformMatrix(glm::vec2 const& translation, glm::vec2 const& scale, float angle)
{
	auto identityMatrix = glm::mat3(1.0f);

	glm::mat3x3 trans{};
	glm::mat3x3 rotation{};
	glm::mat3x3 scaling{};

	trans = glm::translate(identityMatrix, translation);

#ifdef GLM_FORCE_RADIANS
	rotation = glm::rotate(identityMatrix, glm::radians(angle));
#else
	rotation = glm::rotate(identityMatrix, angle);
#endif

	scaling = glm::scale(identityMatrix, scale);

	return trans * rotation * scaling;
}

bool TransformComponent::WasChanged() const
{
	return m_bWasChanged && IS_VALID(m_pParent) ? m_pParent->WasChanged() : true;
}

void TransformComponent::UpdateTransformHierarchy()
{
	if (!WasChanged())
	{
		return;
	}

	if (IS_VALID(m_pParent))
	{
		m_pParent->UpdateTransformHierarchy();

		m_WorldTransformMatrix = m_pParent->m_WorldTransformMatrix * m_LocalTransformMatrix;
	}
	else
	{
		m_LocalTransformMatrix = m_WorldTransformMatrix;
	}

	m_bWasChanged = false;
}