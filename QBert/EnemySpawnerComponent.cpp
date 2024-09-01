#include "PCH.h"
#include "EnemySpawnerComponent.h"

void EnemySpawnerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(EnemySpawnerComponent)).hash_code()));

	Component::Serialize(writer);
}