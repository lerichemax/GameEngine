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
	OnPointsUpdated.Notify(m_NbrPoints); 
}

void CharacterPoint::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterPoint).name());
	writer.WriteInt("points", m_NbrPoints);

	Component::Serialize(writer);
}

void CharacterPoint::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadInt("points", m_NbrPoints);

	Component::Deserialize(reader, context);
}