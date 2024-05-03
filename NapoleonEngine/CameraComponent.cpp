#include "PCH.h"
#include "CameraComponent.h"

#include "GameObject.h"
#include "NapoleonEngine.h"
#include "SceneManager.h"
#include "TransformComponent.h"

CameraComponent::CameraComponent()
	:m_Width(),
	m_Height()
{
	
}

CameraComponent::CameraComponent(CameraComponent const& other)
	:m_Width(other.m_Width),
	m_Height(other.m_Height)
{
	
}

void CameraComponent::Initialize()
{
	m_Width = Renderer::GetInstance().GetWindowWidth();
	m_Height = Renderer::GetInstance().GetWindowHeight();
}

void CameraComponent::Transform() const
{
	glm::vec2 const pos = m_pGameObject->GetTransform()->GetPosition();
	glm::vec2 const upLeft{ pos.x - m_Width/2 , pos.y - m_Height /2 };

	glTranslatef(-upLeft.x, -upLeft.y,0);
}

glm::vec2 CameraComponent::TransformIntoCameraSpace(glm::vec2 const pos)
{
	return TransformPoint(pos, GetCameraMatrix());
}

glm::mat3x3 CameraComponent::GetCameraMatrix() const
{
	auto const trans = m_pGameObject->GetTransform();

	return BuildTransformMatrix(trans->GetWorldPosition(), trans->GetWorldRotation(), 
		trans->GetWorldScale());
}