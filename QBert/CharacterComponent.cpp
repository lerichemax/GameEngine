#include "PCH.h"
#include "CharacterComponent.h"

void CharacterComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterComponent).name());

	Component::Serialize(writer);
}