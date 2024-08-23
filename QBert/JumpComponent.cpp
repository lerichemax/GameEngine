#include "PCH.h"
#include "JumpComponent.h"

void JumpComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(JumpComponent).name());

	Component::Serialize(writer);
}
