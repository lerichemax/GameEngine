#pragma once
#include "Component.h"
#include <array>

class Pyramid;
class Qube;
class Coily;
class SlickSam;
class WrongWay;
class EnemyManager_bu final : public empire::Component
{
public:
	EnemyManager_bu();
	EnemyManager_bu* Clone() override { return nullptr; }//TEMP
	
	void Initialize() override;
	void Update() override;
	
	void SetCoiliesIdle(bool isIdle);
	void EnemyDied(Coily* pCoily);
	void SlickSamDied(SlickSam* pSlickSam);
	void WrongWayDied(WrongWay* pWrongWay);
	void Reset();
	void ResetTimers();
	
	void SetPyramid(Pyramid* pPyramid) { m_pPyramid = pPyramid; }
	
private:
	unsigned int static const MAX_COILY{ 1 };
	float const COILY_SPAWN_INTERVAL{ 4.f };
	unsigned int static const MAX_SLICKSAM{ 2 };
	float const SLICKSAM_SPAWN_INTERVAL{ 10.f };

	unsigned int static const MAX_WRONGWAY{ 2 };
	float const WRONGWAY_SPAWN_INTERVAL{ 7.f };
	
	unsigned int m_NbrSlickSam{};
	unsigned int m_NbrCoily{};
	unsigned int m_NbrWrongWay{};
	
	float m_CoilySpawnTimer{};
	float m_SlickSamSpawnTimer{};
	float m_WrongWaySpawnTimer{};
	
	Pyramid* m_pPyramid;
	
	std::array<Coily*, MAX_COILY> m_pCoilies;
	std::array<SlickSam*, MAX_SLICKSAM> m_pSlickSams;
	std::array<WrongWay*, MAX_WRONGWAY> m_pWrongWays;
	
	void CoilySpawnerTimer();
	void SlickSamSpawnerTimer();
	void WrongWaySpawnerTimer();
	
	void AddCoilyToArray(Coily* pCoily);
	void AddSlickSamToArray(SlickSam* pSlickSam);
	void AddWrongWayToArray(WrongWay* pWrongWay);
	
};