#pragma once
#include "System.h"
#include "Event.h"

class QBert;
class QubeSystem;
class DiskSystem final : public ecs::System
{
public:
	DiskSystem() = default;
	~DiskSystem() = default;

	EventHandler<DiskSystem, Entity> OnDiskReachedTop;

	void SpawnDisk(Entity qubeEntity);

protected:
	void Start() override;
	void Update() override;
};