#pragma once
#include "Component.h"

#define QBERT_TAG "Qbert"

struct AudioComponent;
struct QbertComponent : public ecs::Component
{
	QbertComponent();

	AudioComponent* pJumpSound{};
	AudioComponent* pFallSound{};
	AudioComponent* pSwearSound{};

	float const MAX_RESET_TIME{ 0.75f };
	float ResetTimer{};

	Entity Disk;

	bool bOnResetCoolDown{};

	void Serialize(StreamWriter& writer) const override;
	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;
};