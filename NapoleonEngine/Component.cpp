#include "PCH.h"
#include "Component.h"
#include "GameObject.h"

using namespace ecs;

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
	writer.WriteBool( "IsActive" , m_IsActive);
	writer.WriteBool( "IsUnique" , m_IsUnique);
	writer.WriteInt( "ID" , m_Id);
	writer.WriteInt( "entity" , m_Entity);
}

void Component::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadBool( "IsActive" , m_IsActive);
	reader->ReadBool( "IsUnique" , m_IsUnique);
	reader->ReadInt( "ID" , m_Id);
}

void Component::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int id = -1;
	reader->ReadInt(std::string{ "entity" }, id);

	m_Entity = *context.GetRef<Entity>(id);
}