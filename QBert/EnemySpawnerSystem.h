#pragma once
#include "Component.h"

#include "Event.h"

struct EnemySpawnerComponent;
class PyramidSystem;
class EnemySpawnerSystem : public ecs::System
{
public:
	EnemySpawnerSystem() = default;

	void Reset();

	EventHandler<EnemySpawnerSystem, Entity> OnEnemySpawned;

protected:
	void Start() override;
	void Update() override;
	
	void Spawn(EnemySpawnerComponent* const pSpawnerComp) const;

private:
	PyramidSystem* m_pPyramid;

	bool bIsPaused;

};