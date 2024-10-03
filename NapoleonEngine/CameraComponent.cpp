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
	m_Width = Renderer::Get().GetWindowWidth();
	m_Height = Renderer::Get().GetWindowHeight();
}

void CameraComponent::Transform() const
{
	glm::vec2 const Pos = GetGameObject()->GetTransform()->GetLocation();
	glm::vec2 const upLeft{ Pos.x - m_Width/2 , Pos.y - m_Height /2 };

	glTranslatef(-upLeft.x, -upLeft.y,0);
}

glm::vec2 CameraComponent::TransformIntoCameraSpace(glm::vec2 const Pos)
{
	return TransformPoint(Pos, GetCameraMatrix());
}

glm::mat3x3 CameraComponent::GetCameraMatrix() const
{
	auto const trans = GetGameObject()->GetTransform();

	return BuildTransformMatrix(trans->GetLocation(), trans->GetRotation(),
		trans->GetScale());
}

ECS_CameraComponent::ECS_CameraComponent()
	:m_Width(Renderer::Get().GetWindowWidth()),
	m_Height(Renderer::Get().GetWindowHeight())
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
	int Width, Height;
	reader->ReadInt("width", Width);
	reader->ReadInt("height", Height);
	m_Width = Width;
	m_Height = Height;
}

void CameraSystem::Update()
{
	auto transformComp = m_pRegistry->GetComponent<TransformComponent>(m_MainCameraEntity);
	auto cameraComp = m_pRegistry->GetComponent<ECS_CameraComponent>(m_MainCameraEntity);

	if (!transformComp->IsActive() || !cameraComp->IsActive())
	{
		return;
	}

	glm::vec2 const Pos = transformComp->GetLocation();
	glm::vec2 const upLeft{ Pos.x - cameraComp->m_Width / 2 , Pos.y - cameraComp->m_Height / 2 };

	glTranslatef(-upLeft.x, -upLeft.y, 0);
}

bool CameraSystem::TrySetMainCamera(GameObject* const pGameObject)
{
	auto entityIt = std::find(m_Entities.begin(), m_Entities.end(), pGameObject->GetEntity());
	
	if (entityIt == m_Entities.end())
	{
		Debugger::Get().LogWarning("This entity has no camera component");
		return false;
	}

	m_MainCameraEntity = pGameObject->GetEntity();
	return true;
}

void CameraSystem::SetSignature() const
{
	Signature signature{};
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());
	signature.set(m_pRegistry->GetComponentType<ECS_CameraComponent>());

	m_pRegistry->SetSystemSignature<CameraSystem>(signature);
}