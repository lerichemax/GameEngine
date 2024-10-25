#include "PCH.h"
#include "CameraSystem.h"

#include "CameraComponent.h"

void CameraSystem::Update()
{
	auto transformComp = m_pRegistry->GetComponent<TransformComponent>(m_MainCameraEntity);
	auto cameraComp = m_pRegistry->GetComponent<CameraComponent>(m_MainCameraEntity);

	if (!transformComp->IsActive() || !cameraComp->IsActive())
	{
		return;
	}

	glm::vec2 const Pos = transformComp->GetLocation();
	glm::vec2 const upLeft{ Pos.x - cameraComp->m_Width / 2 , Pos.y - cameraComp->m_Height / 2 };

	glTranslatef(-upLeft.x, -upLeft.y, 0);
}

bool CameraSystem::TrySetMainCamera(Entity cameraEntity)
{
	auto entityIt = std::find(m_Entities.begin(), m_Entities.end(), cameraEntity);

	if (entityIt == m_Entities.end())
	{
		LOG_WARNING("Entity %d has no camera component", cameraEntity);
		return false;
	}

	m_MainCameraEntity = cameraEntity;
	return true;
}

void CameraSystem::SetSignature() const
{
	Signature signature{};
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());
	signature.set(m_pRegistry->GetComponentType<CameraComponent>());

	m_pRegistry->SetSystemSignature<CameraSystem>(signature);
}