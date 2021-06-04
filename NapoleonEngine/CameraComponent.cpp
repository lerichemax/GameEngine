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
	auto& pSceneMan = SceneManager::GetInstance();
	m_Width = pSceneMan.GetEngine()->GetWindowWidth();
	m_Height = pSceneMan.GetEngine()->GetWindowHeight();
}

void CameraComponent::Transform()
{
	glm::vec2 pos = m_pGameObject->GetTransform()->GetPosition();
	glm::vec2 upLeft{ pos.x - m_Width/2 , pos.y - m_Height /2 };

	glTranslatef(-upLeft.x, -upLeft.y,0);
}

glm::vec2 CameraComponent::TransformIntoCameraSpace(glm::vec2 const pos)
{
	glm::vec3 pos3{ pos.x, pos.y, 1 };
	
	glm::vec3 toReturn =  GetCameraMatrix() * pos3;
	return glm::vec2{ toReturn.x, toReturn.y };
}

glm::mat3x3 CameraComponent::GetCameraMatrix()
{
	auto trans = m_pGameObject->GetTransform();
	auto pos = trans->GetWorldPosition();
	glm::vec2 camPos{ pos.x - m_Width / 2 , pos.y - m_Height / 2 };

	return BuildTransformMatrix(trans->GetWorldPosition(), trans->GetWorldRotation(), 
		trans->GetWorldScale());
}