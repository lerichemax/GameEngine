#pragma once
#include "Component.h"

struct EnemySpawnerComponent;
class PyramidSystem;
class EnemySpawnerSystem : public System
{
public:
	EnemySpawnerSystem() = default;

	void Reset();
	void ResetTimer();

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Start() override;
	void Update() override;
	
	void Spawn(EnemySpawnerComponent* const pSpawnerComp) const;

private:
	PyramidSystem* m_pPyramid;
};