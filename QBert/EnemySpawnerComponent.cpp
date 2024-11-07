#include "PCH.h"
#include "EnemySpawnerComponent.h"

void EnemySpawnerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString(std::string{ "type" }, typeid(EnemySpawnerComponent).name());

	Component::Serialize(writer);
}