#include "PCH.h"
#include "UiManagerComponent.h"

void UiManagerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString(std::string{ "type" }, typeid(UiManagerComponent).name());

	Component::Serialize(writer);
}