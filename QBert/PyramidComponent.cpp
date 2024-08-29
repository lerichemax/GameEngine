#include "PCH.h"
#include "PyramidComponent.h"

void PyramidComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(PyramidComponent)).hash_code()));

	Component::Serialize(writer);
}