#include "PCH.h"
#include "CoilyComponent.h"
#include "QubeComponent.h"

CoilyComponent::CoilyComponent()
{
	for (size_t i = 0; i < MOVEMENT_QUEUE_SIZE; i++)
	{
		MovementQueue[i] = ConnectionDirection::null;
	}
}

void CoilyComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(CoilyComponent)).hash_code()));

	Component::Serialize(writer);
}