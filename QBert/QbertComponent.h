#pragma once
#include "Component.h"

#define QBERT_TAG "Qbert"

struct AudioComponent;
struct QbertComponent : public ecs::Component
{
	QbertComponent();

	PROPERTY(AudioComponent*, pJumpSound);
	PROPERTY(AudioComponent*, pFallSound);
	PROPERTY(AudioComponent*, pSwearSound);

	float const MAX_RESET_TIME{ 0.75f };
	float ResetTimer{};

	Entity Disk;

	bool bOnResetCoolDown{};
};

SERIALIZE_CLASS(QbertComponent, ecs::Component)