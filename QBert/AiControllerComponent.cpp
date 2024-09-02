#include "PCH.h"
#include "AiControllerComponent.h"

void AiControllerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(AiControllerComponent)).hash_code()));

	Component::Serialize(writer);
}