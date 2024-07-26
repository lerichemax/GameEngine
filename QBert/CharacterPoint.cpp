#include "PCH.h"
#include "CharacterPoint.h"

CharacterPoint::CharacterPoint()
	:m_NbrPoints()
{
	
}

ECS_CharacterPoint::ECS_CharacterPoint()
	:ECS_Component(true),
	m_NbrPoints()
{

}

void ECS_CharacterPoint::AddPoints(int nbrPoint)
{ 
	m_NbrPoints += nbrPoint; 
	OnPointsUpdated.Notify(m_NbrPoints); 
}

void ECS_CharacterPoint::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(ECS_CharacterPoint).name());
	writer.WriteInt("points", m_NbrPoints);

	ECS_Component::Serialize(writer);
}

void ECS_CharacterPoint::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadInt("points", m_NbrPoints);

	ECS_Component::Deserialize(reader, context);
}