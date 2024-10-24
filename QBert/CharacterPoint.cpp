#include "PCH.h"
#include "CharacterPoint.h"

CharacterPoint::CharacterPoint()
	:Component(true),
	m_NbrPoints()
{

}

void CharacterPoint::AddPoints(int nbrPoint)
{ 
	m_NbrPoints += nbrPoint; 
	OnPointsUpdated.Notify(m_NbrPoints, GetEntity()); 
}

void CharacterPoint::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(CharacterPoint)).hash_code()));
	writer.WriteInt("points", m_NbrPoints);

	Component::Serialize(writer);
}

void CharacterPoint::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadInt("points", m_NbrPoints);

	Component::Deserialize(reader, context);
}