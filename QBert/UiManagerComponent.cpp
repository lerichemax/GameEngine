#include "PCH.h"
#include "UiManagerComponent.h"

void UiManagerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(UiManagerComponent)).hash_code()));

	Component::Serialize(writer);
}