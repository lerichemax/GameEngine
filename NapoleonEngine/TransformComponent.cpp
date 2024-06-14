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
	//std::shared_ptr<GameObject> pParent = m_pGameObject->GetParent();
	//if (pParent != nullptr)
	//{
	//	/*auto const parentWorld = pParent->GetECSTransform()->m_WorldPosition;
	//	m_WorldPosition = parentWorld + m_Position;
	//	m_WorldRotation = pParent->GetECSTransform()->m_WorldRotation + m_Rotation;
	//	m_WorldScale = pParent->GetECSTransform()->m_WorldScale * m_Scale;*/
	//}
	//else
	//{
	//	m_WorldPosition = m_Position;
	//	m_WorldRotation = m_Rotation;
	//	m_WorldScale = m_Scale;
	//}

	//m_World = BuildTransformMatrix(m_WorldPosition, m_WorldRotation, m_WorldScale);
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
	//auto const parentObj = m_pGameObject->GetParent();
	//if (parentObj == nullptr)
	//{
		Translate(worldPos);
	//}
	//else
	//{
	//	auto const parentWorld = parentObj->GetTransform()->GetWorldPosition();

	//	auto const pos = worldPos - parentWorld;

	//	Translate(pos);
	//}
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

//void ECS_TransformComponent::SetLocation(vec2 const& loc)
//{
//	m_OldPosition = m_Position;
//
//	m_Position = loc;
//}
//
//void ECS_TransformComponent::SetLocation(float x, float y)
//{
//
//}
//
//void ECS_TransformComponent::SetLocalLocation(vec2 const& loc)
//{
//
//}
//
//void ECS_TransformComponent::SetLocalLocation(float x, float y)
//{
//
//}

void ECS_TransformComponent::Translate(vec2 const& translation)
{
	m_OldPosition = m_Position;

	m_Position += translation;
}

void ECS_TransformComponent::Translate(float x, float y)
{
	Translate(vec2{ x,y });
}

void ECS_TransformComponent::Scale(vec2 const& scale)
{
	m_OldScale = m_Scale;

	m_Scale = {scale.x, scale.y};
}

void ECS_TransformComponent::Scale(float scale)
{
	Scale({ scale, scale });
}

void ECS_TransformComponent::Rotate(float rotation)
{
	m_OldRotation = m_Rotation;
	m_Rotation = rotation;
}

bool ECS_TransformComponent::HasChanged() const
{
	return m_Position != m_OldPosition || m_Scale != m_OldScale || m_Rotation != m_OldRotation;
}

void ECS_TransformComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(ECS_TransformComponent).name());
	writer.WriteInt("parent", m_pParent == nullptr ? -1 : m_pParent->GetId());
	writer.StartObject("position");
	{
		writer.WriteDouble("x", m_Position.x);
		writer.WriteDouble("y", m_Position.y);
	}
	writer.EndObject();

	writer.StartObject("scale");
	{
		writer.WriteDouble("x", m_Scale.x);
		writer.WriteDouble("y", m_Scale.y);
	}
	writer.EndObject();

	writer.WriteDouble("rotation", m_Rotation);

	writer.StartObject("world_position");
	{
		writer.WriteDouble("x", m_WorldPosition.x);
		writer.WriteDouble("y", m_WorldPosition.y);
	}
	writer.EndObject();

	writer.StartObject("world_scale");
	{
		writer.WriteDouble("x", m_WorldScale.x);
		writer.WriteDouble("y", m_WorldScale.y);
	}
	writer.EndObject();

	writer.WriteDouble("world_rotation", m_WorldRotation);
}

void ECS_TransformComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	auto posObject = reader->ReadObject("position");
	posObject->ReadDouble("x", m_Position.x);
	posObject->ReadDouble("y", m_Position.y);

	auto rotObject = reader->ReadObject("scale");
	rotObject->ReadDouble("x", m_Scale.x);
	rotObject->ReadDouble("y", m_Scale.y);

	reader->ReadDouble("rotation", m_Rotation);

	auto worlPosObject = reader->ReadObject("world_position");
	worlPosObject->ReadDouble("x", m_WorldPosition.x);
	worlPosObject->ReadDouble("y", m_WorldPosition.y);

	auto scaleObject = reader->ReadObject("world_scale");
	scaleObject->ReadDouble("x", m_WorldScale.x);
	scaleObject->ReadDouble("y", m_WorldScale.y);

	reader->ReadDouble("world_rotation", m_WorldRotation);

	ECS_Component::Deserialize(reader, context);
}

void ECS_TransformComponent::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int parent = 0;
	reader->ReadInt("parent", parent);
	m_pParent = context.GetRef<ECS_TransformComponent>(parent);
}

void TransformSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto transComp = pComponentManager->GetComponent<ECS_TransformComponent>(entity);

		if (!transComp->IsActive())
		{
			return;
		}

		RecursivelyUpdateHierarchy(transComp);

		transComp->m_World = BuildTransformMatrix(transComp->m_WorldPosition, transComp->m_WorldRotation, transComp->m_WorldScale);
	}
}

void TransformSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());

	pRegistry->SetSystemSignature<TransformSystem>(signature);
}

void TransformSystem::RecursivelyUpdateHierarchy(std::shared_ptr<ECS_TransformComponent> transformComponent) const
{
	if (!transformComponent->HasChanged())
	{
		return;
	}

	if (transformComponent->m_pParent != nullptr)
	{
		RecursivelyUpdateHierarchy(transformComponent->m_pParent);

		auto const parentWorld = transformComponent->m_pParent->m_WorldPosition;

		transformComponent->m_WorldPosition = parentWorld + transformComponent->m_Position;
		transformComponent->m_WorldRotation = transformComponent->m_pParent->m_WorldRotation + transformComponent->m_Rotation;
		transformComponent->m_WorldScale = transformComponent->m_pParent->m_WorldScale * transformComponent->m_Scale;
	}
	else
	{
		transformComponent->m_WorldPosition = transformComponent->m_Position;
		transformComponent->m_WorldRotation = transformComponent->m_Rotation;
		transformComponent->m_WorldScale = transformComponent->m_Scale;
	}

	transformComponent->m_OldPosition = transformComponent->m_Position;
	transformComponent->m_OldRotation = transformComponent->m_Rotation;
	transformComponent->m_OldScale = transformComponent->m_Scale;
}