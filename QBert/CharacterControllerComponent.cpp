#include "PCH.h"
#include "CharacterControllerComponent.h"

void CharacterControllerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(CharacterControllerComponent)).hash_code()));

	Component::Serialize(writer);
}