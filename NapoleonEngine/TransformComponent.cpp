#include "PCH.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include <glm/gtx/matrix_transform_2d.hpp>

//TransformComponent::TransformComponent()
//	:m_Location(),
//	m_Scale(1, 1),
//	m_Rotation(),
//	m_World(),
//	m_WorldLocation(),
//	m_WorldScale(),
//	m_WorldRotation()
//{
//	
//}
//
//
//TransformComponent::TransformComponent(float x, float y)
//	:m_Location{x,y},
//	m_Scale(1, 1),
//	m_Rotation(),
//	m_World(),
//	m_WorldLocation(),
//	m_WorldScale(),
//	m_WorldRotation()
//	
//{
//}
//
//void TransformComponent::Initialize()
//{
//	Update();
//}
//
//void TransformComponent::Update()
//{
//	//std::shared_ptr<GameObject> pParent = m_pGameObject->GetParent();
//	//if (pParent != nullptr)
//	//{
//	//	/*auto const parentWorld = pParent->GetECSTransform()->m_WorldLocation;
//	//	m_WorldLocation = parentWorld + m_Location;
//	//	m_WorldRotation = pParent->GetECSTransform()->m_WorldRotation + m_Rotation;
//	//	m_WorldScale = pParent->GetECSTransform()->m_WorldScale * m_Scale;*/
//	//}
//	//else
//	//{
//	//	m_WorldLocation = m_Location;
//	//	m_WorldRotation = m_Rotation;
//	//	m_WorldScale = m_Scale;
//	//}
//
//	//m_World = BuildTransformMatrix(m_WorldLocation, m_WorldRotation, m_WorldScale);
//}
//
//void TransformComponent::Translate(const float x, const float y)
//{
//	m_Location.x = x;
//	m_Location.y = y;
//}
//
//void TransformComponent::Translate(glm::vec2 const& pos)
//{
//	m_Location = pos;
//}
//
//void TransformComponent::SetWorldPosition(float x, float y)
//{
//	SetWorldPosition(glm::vec2{ x,y });
//}
//
//void TransformComponent::SetWorldPosition(glm::vec2 const& worldPos)
//{
//	//auto const parentObj = m_pGameObject->GetParent();
//	//if (parentObj == nullptr)
//	//{
//		Translate(worldPos);
//	//}
//	//else
//	//{
//	//	auto const parentWorld = parentObj->GetTransform()->GetWorldLocation();
//
//	//	auto const pos = worldPos - parentWorld;
//
//	//	Translate(pos);
//	//}
//}
//
//void TransformComponent::Scale(float x, float y)
//{
//	m_Scale.x = x;
//	m_Scale.y = y;
//}
//
//void TransformComponent::Scale(glm::vec2 const& scale)
//{
//	m_Scale.x = scale.x;
//	m_Scale.y = scale.y;
//}
//
//void TransformComponent::Scale(float uniformScale)
//{
//	m_Scale.x = uniformScale;
//	m_Scale.y = uniformScale;
//}
//
//TransformComponent& TransformComponent::operator=(TransformComponent const& rhs)
//{
//	m_Location.x = rhs.m_Location.x;
//	m_Location.y = rhs.m_Location.y;
//
//	m_WorldLocation.x = rhs.m_WorldLocation.x;
//	m_WorldLocation.y = rhs.m_WorldLocation.y;
//
//	m_Scale.x = rhs.m_Scale.x;
//	m_Scale.y = rhs.m_Scale.y;
//
//	m_Rotation = rhs.m_Rotation;
//
//	m_WorldRotation = rhs.m_WorldRotation;
//	m_WorldScale = rhs.m_WorldScale;
//
//	m_World = rhs.m_World;
//	
//	return *this;
//}
//
//void TransformComponent::Rotate(float rot)
//{
//	m_Rotation = rot;
//}

//void TransformComponent::SetLocation(vec2 const& loc)
//{
//	m_OldPosition = m_Location;
//
//	m_Location = loc;
//}
//
//void TransformComponent::SetLocation(float x, float y)
//{
//
//}
//
void TransformComponent::SetLocalLocation(vec2 const& loc)
{
	m_Location = loc;

	if (m_pParent != nullptr)
	{
		m_WorldLocation = m_pParent->m_WorldLocation + m_Location;
	}
	else
	{
		m_WorldLocation = m_Location;
	}
}

//void TransformComponent::SetLocalLocation(float x, float y)
//{
//
//}

void TransformComponent::Translate(vec2 const& translation)
{
	m_OldPosition = m_WorldLocation;

	m_WorldLocation = translation;
	if (m_pParent != nullptr)
	{
		m_Location = m_WorldLocation - m_pParent->m_WorldLocation;
	}
	else
	{
		m_Location = m_WorldLocation;
	}
}

void TransformComponent::Translate(float x, float y)
{
	Translate(vec2{ x,y });
}

void TransformComponent::Scale(vec2 const& scale)
{
	m_OldScale = m_WorldScale;

	m_WorldScale = {scale.x, scale.y};

	if (m_pParent != nullptr)
	{
		m_Scale = m_WorldScale / m_pParent->m_WorldScale;
	}
	else
	{
		m_Scale = m_WorldScale;
	}
}

void TransformComponent::Scale(float scale)
{
	Scale({ scale, scale });
}

void TransformComponent::Rotate(float rotation)
{
	m_OldRotation = m_WorldRotation;
	m_WorldRotation = rotation;

	if (m_pParent != nullptr)
	{
		m_Rotation = m_WorldRotation - m_pParent->m_WorldRotation;
	} 
}

void TransformComponent::SetParent(TransformComponent* const pParent)
{
	m_pParent = pParent;
	Translate(m_WorldLocation);
	Rotate(m_WorldRotation);
	Scale(m_WorldScale);
}

bool TransformComponent::HasChanged() const
{
	return m_Location != m_OldPosition || m_Scale != m_OldScale || m_Rotation != m_OldRotation;
}

void TransformComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(TransformComponent)).hash_code()));
	writer.WriteInt("parent", m_pParent == nullptr ? -1 : m_pParent->GetId());
	writer.StartObject("position");
	{
		writer.WriteDouble("x", m_Location.x);
		writer.WriteDouble("y", m_Location.y);
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
		writer.WriteDouble("x", m_WorldLocation.x);
		writer.WriteDouble("y", m_WorldLocation.y);
	}
	writer.EndObject();

	writer.StartObject("world_scale");
	{
		writer.WriteDouble("x", m_WorldScale.x);
		writer.WriteDouble("y", m_WorldScale.y);
	}
	writer.EndObject();

	writer.WriteDouble("world_rotation", m_WorldRotation);
	
	Component::Serialize(writer);
}

void TransformComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	std::string x{ "x" };
	std::string y{ "y" };
	auto posObject = reader->ReadObject(std::string{ "position" });
	posObject->ReadDouble(x, m_Location.x);
	posObject->ReadDouble(y, m_Location.y);

	auto rotObject = reader->ReadObject(std::string{ "scale" });
	rotObject->ReadDouble(x, m_Scale.x);
	rotObject->ReadDouble(y, m_Scale.y);

	reader->ReadDouble(std::string{ "rotation" }, m_Rotation);

	auto worlPosObject = reader->ReadObject(std::string{ "world_position" });
	worlPosObject->ReadDouble(x, m_WorldLocation.x);
	worlPosObject->ReadDouble(y, m_WorldLocation.y);

	auto scaleObject = reader->ReadObject(std::string{ "world_scale" });
	scaleObject->ReadDouble(x, m_WorldScale.x);
	scaleObject->ReadDouble(y, m_WorldScale.y);

	reader->ReadDouble(std::string{ "world_rotation" }, m_WorldRotation);

	Component::Deserialize(reader, context);
}

void TransformComponent::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int parent = -1;
	reader->ReadInt(std::string{ "parent" }, parent);
	m_pParent = context.GetRef<TransformComponent>(parent);
}
