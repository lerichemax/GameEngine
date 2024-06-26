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

	return BuildTransformMatrix(trans->GetPosition(), trans->GetRotation(),
		trans->GetScale());
}

ECS_CameraComponent::ECS_CameraComponent()
	:m_Width(Renderer::GetInstance().GetWindowWidth()),
	m_Height(Renderer::GetInstance().GetWindowHeight())
{
}

void ECS_CameraComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(ECS_CameraComponent).name());
	writer.WriteInt("width", m_Width);
	writer.WriteInt("height", m_Height);
}

void ECS_CameraComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	int width, height;
	reader->ReadInt("width", width);
	reader->ReadInt("height", height);
	m_Width = width;
	m_Height = height;
}

void CameraSystem::Update(ComponentManager* const pComponentManager)
{
	auto transformComp = pComponentManager->GetComponent<ECS_TransformComponent>(m_MainCameraEntity);
	auto cameraComp = pComponentManager->GetComponent<ECS_CameraComponent>(m_MainCameraEntity);

	if (!transformComp->IsActive() || !cameraComp->IsActive())
	{
		return;
	}

	glm::vec2 const pos = transformComp->GetPosition();
	glm::vec2 const upLeft{ pos.x - cameraComp->m_Width / 2 , pos.y - cameraComp->m_Height / 2 };

	glTranslatef(-upLeft.x, -upLeft.y, 0);
}

bool CameraSystem::TrySetMainCamera(std::shared_ptr<GameObject> pGameObject)
{
	auto entityIt = std::find(m_Entities.begin(), m_Entities.end(), pGameObject->GetEntity());
	
	if (entityIt == m_Entities.end())
	{
		Debugger::GetInstance().LogWarning("This entity has no camera component");
		return false;
	}

	m_MainCameraEntity = pGameObject->GetEntity();
	return true;
}

void CameraSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature{};
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());
	signature.set(pRegistry->GetComponentType<ECS_CameraComponent>());

	pRegistry->SetSystemSignature<CameraSystem>(signature);
}