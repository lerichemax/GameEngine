#include "PCH.h"
#include "Component.h"

Component::Component()
	:m_pGameObject{},
	m_bIsEnabled(false)
{
}

void Component::RootInitialize(GameObject* pParent)
{
	m_pGameObject = pParent;
	m_bIsEnabled = true;
}

ECS_Component::ECS_Component(bool unique)
	:m_IsActive{true},
	m_IsUnique{ unique }
{
}

bool ECS_Component::IsActive() const
{
	return m_IsActive;
}

bool ECS_Component::IsUnique() const
{
	return m_IsUnique;
}

void ECS_Component::SetActive(bool isActive)
{
	m_IsActive = isActive;
	m_NeedsUpdate = true;
}

void ECS_Component::Serialize(StreamWriter& writer) const
{
	writer.StartArrayObject(); //temp
	writer.WriteBool("IsActive", m_IsActive);
	writer.WriteBool("IsUnique", m_IsUnique);
	writer.EndObject();
}

void ECS_Component::Deserialize(JsonReader const* reader)
{
	reader->ReadBool("IsActive", m_IsActive);
	reader->ReadBool("IsUnique", m_IsUnique);
}