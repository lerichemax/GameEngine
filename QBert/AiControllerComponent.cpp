#include "PCH.h"
#include "AiControllerComponent.h"

void AiControllerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString(std::string{ "type" }, typeid(AiControllerComponent).name());
	writer.WriteInt("enemyType", static_cast<int>(Type));
	writer.WriteInt("points", PointsForKill);

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

	reader->ReadInt("points", PointsForKill);

	Component::Deserialize(reader, context);
}