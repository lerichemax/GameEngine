#include "PCH.h"
#include "Command.h"

void Command::Serialize(StreamWriter& writer) const
{
	writer.WriteInt("id", m_Id);
}

void Command::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	reader->ReadInt("id", m_Id);
}