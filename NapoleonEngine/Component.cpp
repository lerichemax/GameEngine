#include "PCH.h"
#include "Component.h"
#include "GameObject.h"

Component::Component(bool unique)
	:m_IsActive{true},
	m_IsUnique{ unique }
{
}

bool Component::IsActive() const
{
	return m_IsActive;
}

bool Component::IsUnique() const
{
	return m_IsUnique;
}

void Component::SetActive(bool isActive)
{
	m_IsActive = isActive;
}

void Component::Serialize(StreamWriter& writer) const
{
	writer.WriteBool(std::string{ "IsActive" }, m_IsActive);
	writer.WriteBool(std::string{ "IsUnique" }, m_IsUnique);
	writer.WriteInt(std::string{ "ID" }, m_Id);
	writer.WriteInt(std::string{ "gameobject" }, m_pGameObject->GetEntity());
}

void Component::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadBool(std::string{ "IsActive" }, m_IsActive);
	reader->ReadBool(std::string{ "IsUnique" }, m_IsUnique);
	reader->ReadInt(std::string{ "ID" }, m_Id);
}

void Component::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int id = -1;
	reader->ReadInt(std::string{ "gameobject" }, id);

	m_pGameObject = context.GetRef<GameObject>(id);
}