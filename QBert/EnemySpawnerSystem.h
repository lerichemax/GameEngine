#pragma once
#include "Component.h"

class PyramidSystem;
class EnemySpawnerSystem : public System
{
public:
	EnemySpawnerSystem() = default;

	void EnemyDied(Enemy* pEnemy);
	void Reset();
	void ResetTimer();

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Start() override;
	void Update() override;
	
	void Spawn(EnemySpawnerComponent* const pSpawnerComp);

private:
	PyramidSystem* m_pPyramid;
};