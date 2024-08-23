#include "PCH.h"
#include "CharacterLives.h"

CharacterLives::CharacterLives()
	:Component(true),
	m_NbrLives(MAX_LIVES)
{

}

void CharacterLives::Die()
{
	m_NbrLives--;
	OnDied.Notify(m_NbrLives);

	if (m_NbrLives <= 0)
	{
		OnGameOver.Notify();
	}
}

void CharacterLives::Reset()
{
	m_NbrLives = MAX_LIVES;
}

void CharacterLives::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterLives).name());
	writer.WriteInt("lives", m_NbrLives);
	
	Component::Serialize(writer);
}

void CharacterLives::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadInt("lives", m_NbrLives);

	Component::Deserialize(reader, context);
}