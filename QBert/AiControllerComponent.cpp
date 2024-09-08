#include "PCH.h"
#include "AiControllerComponent.h"

void AiControllerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(AiControllerComponent)).hash_code()));
	writer.WriteInt("enemyType", static_cast<int>(Type));

	Component::Serialize(writer);
}

void AiControllerComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	int type = -1;
	reader->ReadInt("enemyType", type);

	if (type != -1 && type < static_cast<int>(EnemyType::MAX))
	{
		Type = static_cast<EnemyType>(type);
	}

	Component::Deserialize(reader, context);
}