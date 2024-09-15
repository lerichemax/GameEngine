#pragma once
#include "Component.h"

#include "Event.h"

struct EnemySpawnerComponent;
class PyramidSystem;
class EnemySpawnerSystem : public System
{
public:
	EnemySpawnerSystem() = default;

	void ResetTimer();

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

	EventHandler<EnemySpawnerSystem, Entity> OnEnemySpawned;

protected:
	void Start() override;
	void Update() override;
	
	void Spawn(EnemySpawnerComponent* const pSpawnerComp) const;

private:
	PyramidSystem* m_pPyramid;

	bool bIsPaused;

	void Reset();
};