#include "PCH.h"
#include "CharacterComponent.h"

void CharacterComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(CharacterComponent)).hash_code()));

	Component::Serialize(writer);
}