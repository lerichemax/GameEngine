#include "PCH.h"
#include "Component.h"
#include "GameObject.h"

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
}

void ECS_Component::Serialize(StreamWriter& writer) const
{
	writer.WriteBool("IsActive", m_IsActive);
	writer.WriteBool("IsUnique", m_IsUnique);
	writer.WriteInt("ID", m_Id);
	writer.WriteInt("gameobject", m_pGameObject->GetEntity());
}

void ECS_Component::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadBool("IsActive", m_IsActive);
	reader->ReadBool("IsUnique", m_IsUnique);
	reader->ReadInt("ID", m_Id);
}

void ECS_Component::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int id = -1;
	reader->ReadInt("gameobject", id);

	m_pGameObject = context.GetRef<GameObject>(id).get();
}