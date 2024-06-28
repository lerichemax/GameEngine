#include "PCH.h"
#include "CharacterLives.h"

CharacterLives::CharacterLives(int maxLives)
	:MAX_LIVES(maxLives),
	m_NbrLives(maxLives),
	m_bIsGameOver(false)
{
	
}

void CharacterLives::Die()
{
	m_NbrLives--;
	if (m_NbrLives <= 0)
	{
		m_bIsGameOver = true;
	}
}

void CharacterLives::Reset()
{
	m_NbrLives = MAX_LIVES;
	m_bIsGameOver = false;
}

ECS_CharacterLives::ECS_CharacterLives(int maxLives)
	:ECS_Component(true),
	m_MaxLives(maxLives),
	m_NbrLives(maxLives)
{

}

void ECS_CharacterLives::Die()
{
	m_NbrLives--;
}

void ECS_CharacterLives::Reset()
{
	m_NbrLives = m_MaxLives;
}

void ECS_CharacterLives::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(ECS_CharacterLives).name());
	writer.WriteInt("maxLives", m_MaxLives);
	writer.WriteInt("lives", m_NbrLives);
	
	ECS_Component::Serialize(writer);
}

void ECS_CharacterLives::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadInt("maxLives", m_MaxLives);
	reader->ReadInt("lives", m_NbrLives);

	ECS_Component::Deserialize(reader, context);
}