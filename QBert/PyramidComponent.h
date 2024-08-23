#pragma once
#include "Component.h"

struct PyramidComponent : public Component
{
	unsigned int const MAX_WIDTH{ 7 };
	unsigned int const MAX_NBR_DISKS{ 4 };
	float const DISK_SPAWNING_INTERVAL{ 7.f };

	unsigned int m_NbrDisksSpawned;

	float m_DiskSpawnTimer;

	std::vector<Entity> m_Qubes;

	void Serialize(StreamWriter& writer) const override;
};