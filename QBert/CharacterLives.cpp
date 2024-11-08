#include "PCH.h"
#include "CharacterLives.h"

CharacterLives::CharacterLives()
	:Component(true)
{

}

void CharacterLives::Init(int nbrLives)
{
	MaxLives = nbrLives;
	NbrLives = MaxLives;
}

void CharacterLives::Die()
{
	NbrLives--;
	bIsDead = true;
}

void CharacterLives::Reset()
{
	NbrLives = MaxLives;
	bIsDead = false;
}

void CharacterLives::Serialize(StreamWriter& writer) const
{
	writer.WriteString(std::string{ "type" }, typeid(CharacterLives).name());
	writer.WriteInt("lives", NbrLives);
	writer.WriteInt("maxLives", MaxLives);
	
	Component::Serialize(writer);
}

void CharacterLives::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadInt("lives", NbrLives);
	reader->ReadInt("maxLives", MaxLives);

	Component::Deserialize(reader, context);
}