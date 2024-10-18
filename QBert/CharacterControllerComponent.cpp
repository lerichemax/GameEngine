#include "PCH.h"
#include "CharacterControllerComponent.h"

void CharacterControllerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(CharacterControllerComponent)).hash_code()));

	writer.WriteInt("right", static_cast<int>(Right));
	writer.WriteInt("left", static_cast<int>(Left));
	writer.WriteInt("up", static_cast<int>(Up));
	writer.WriteInt("down", static_cast<int>(Down));

	Component::Serialize(writer);
}

void CharacterControllerComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	int tmpInt{-1};

	reader->ReadInt("right", tmpInt);
	Right = static_cast<SDL_Scancode>(tmpInt);
	reader->ReadInt("left", tmpInt);
	Left = static_cast<SDL_Scancode>(tmpInt);
	reader->ReadInt("up", tmpInt);
	Up = static_cast<SDL_Scancode>(tmpInt);
	reader->ReadInt("down", tmpInt);
	Down = static_cast<SDL_Scancode>(tmpInt);

	Component::Deserialize(reader, context);
}