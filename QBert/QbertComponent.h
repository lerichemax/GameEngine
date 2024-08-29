#pragma once
#include "Component.h"

struct AudioComponent;
struct QbertComponent : public Component
{
	AudioComponent* pJumpSound;
	AudioComponent* pFallSound;
	AudioComponent* pSwearSound;

	void Serialize(StreamWriter& writer) const override;
	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;
};