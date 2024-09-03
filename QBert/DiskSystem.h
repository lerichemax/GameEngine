#pragma once
#include "System.h"
#include "Event.h"

class QBert;
class QubeSystem;
class DiskSystem final : public System
{
public:
	DiskSystem() = default;
	~DiskSystem() = default;

	EventHandler<DiskSystem, Entity> OnDiskReachedTop;

	void SpawnDisk(Entity qubeEntity);

	void SetSignature() override;

	void Serialize(StreamWriter& writer) const override;

protected:
	void Update() override;
};