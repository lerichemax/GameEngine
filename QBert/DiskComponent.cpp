#include "PCH.h"
#include "DiskComponent.h"

void DiskComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString(std::string{ "type" }, typeid(DiskComponent).name());

	Component::Serialize(writer);
}
