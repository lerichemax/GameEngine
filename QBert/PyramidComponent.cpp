#include "PCH.h"
#include "PyramidComponent.h"

void PyramidComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString(std::string{ "type" }, typeid(PyramidComponent).name());

	Component::Serialize(writer);
}