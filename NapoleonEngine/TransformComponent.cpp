#include "PCH.h"
#include "TransformComponent.h"

#include "GameObject.h"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/matrix_decompose.hpp>

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
}

glm::vec2 TransformComponent::GetLocation() const 
{ 
	return m_WorldTransformMatrix[2]; 
}
glm::vec2 TransformComponent::GetLocalLocation() const 
{ 
	return m_LocalTransformMatrix[2];
}
glm::vec2 TransformComponent::GetScale() const 
{ 
	vec2 scale{};
	scale.x = glm::sqrt(glm::pow(m_WorldTransformMatrix[0][0], 2.f) + glm::pow(m_WorldTransformMatrix[0][1], 2.f));
	scale.y = glm::sqrt(glm::pow(m_WorldTransformMatrix[1][0], 2.f) + glm::pow(m_WorldTransformMatrix[1][1], 2.f));

	return scale;
}
glm::vec2 TransformComponent::GetLocalScale() const 
{ 
	vec2 scale{};
	scale.x = glm::sqrt(glm::pow(m_LocalTransformMatrix[0][0], 2.f) + glm::pow(m_LocalTransformMatrix[0][1], 2.f));
	scale.y = glm::sqrt(glm::pow(m_LocalTransformMatrix[1][0], 2.f) + glm::pow(m_LocalTransformMatrix[1][1], 2.f));

	return scale;
}
float TransformComponent::GetRotation() const 
{ 
	float angle = glm::atan(m_WorldTransformMatrix[0][1], m_WorldTransformMatrix[0][0]);
	return angle;
}
float TransformComponent::GetLocalRotation() const 
{
	float angle = glm::atan(m_LocalTransformMatrix[0][1], m_LocalTransformMatrix[0][0]);
	return angle;
}

void TransformComponent::SetParent(TransformComponent* const pParent)
{
	if (!IS_VALID(pParent))
	{
		return;
	}

	m_pParent = pParent;
	m_LocalTransformMatrix = glm::inverse(m_pParent->m_WorldTransformMatrix) * m_WorldTransformMatrix;
}


void TransformComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(TransformComponent)).hash_code()));
	writer.WriteInt("parent", m_pParent == nullptr ? -1 : m_pParent->GetId());

	vec2 location = GetLocalLocation();
	vec2 scale = GetLocalScale();
	float rotation = GetLocalRotation();

	writer.WriteVector("position", location);
	writer.WriteVector("scale", scale);

	writer.WriteDouble("rotation", rotation);

	location = GetLocation();
	scale = GetScale();
	rotation = GetRotation();

	writer.WriteVector("world_position", location);
	writer.WriteVector("world_scale", scale);

	writer.WriteDouble("world_rotation", rotation);
	
	Component::Serialize(writer);
}

void TransformComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	vec2 location;
	vec2 scale;
	float rotation;

	reader->ReadVector("position", location);
	reader->ReadVector("scale", scale);

	reader->ReadDouble(std::string{ "rotation" }, rotation);

	m_LocalTransformMatrix = BuildTransformMatrix(location, scale, rotation);

	reader->ReadVector("world_position", location);
	reader->ReadVector("world_scale", scale);

	reader->ReadDouble(std::string{ "world_rotation" }, rotation);

	m_WorldTransformMatrix = BuildTransformMatrix(location, scale, rotation);

	Component::Deserialize(reader, context);
}

void TransformComponent::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int parent = -1;
	reader->ReadInt(std::string{ "parent" }, parent);
	m_pParent = context.GetRef<TransformComponent>(parent);
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
