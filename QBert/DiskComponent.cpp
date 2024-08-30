#include "PCH.h"
#include "DiskComponent.h"

void DiskComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(DiskComponent)).hash_code()));

	Component::Serialize(writer);
}
