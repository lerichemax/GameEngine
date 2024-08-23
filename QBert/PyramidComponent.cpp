#include "PCH.h"
#include "PyramidComponent.h"

void PyramidComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(PyramidComponent).name());
}