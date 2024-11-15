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

