#pragma once
#include "Component.h"

#include <vector>
#include <glm/glm.hpp>

using namespace empire;

class EnemyManager;
class Qube;
enum class ConnectionDirection;
class QBert;
class ColoredDisk;
class Pyramid final : public Component
{

public:
	Pyramid(unsigned int maxWidth, QBert* pQbert);
	
	Pyramid* Clone() override { return nullptr; } //unclonable
	
	Pyramid(Pyramid&& other) = delete;
	Pyramid& operator=(Pyramid const& rhs) = delete;
	Pyramid& operator=(Pyramid&& rhs) = delete;
	~Pyramid();

	void Update() override;
	
	Qube* GetTop() const { return m_pQubes.front(); }
	Qube* GetEscheresqueLeftTop() const { return m_pQubes.back(); }
	Qube* GetEscheresqueRightTop() const { return m_pQubes[m_pQubes.size() - MAX_WIDTH]; }
	Qube* GetQube(int index) const { return m_pQubes[index]; }
	EnemyManager* GetEnemyManager() const { return m_pEnemyManager; }
	std::vector<Qube*> const& GetQubes() const { return m_pQubes; }
	
	bool AreAllQubesFlipped() const;
	void Reset();
	void PartialReset();
	void DiskUsed() { m_NbrDisksSpawned--; }
	bool FindNextQubeToQbert(Qube* const pStartingQube, ConnectionDirection* directions, const int size) const; //used by coily to move to qbert
	
private:
	unsigned int const MAX_WIDTH;
	unsigned int const MAX_NBR_DISKS{ 4 };
	float const DISK_SPAWNING_INTERVAL{ 7.f };
	
	unsigned int m_NbrDisksSpawned;
	
	float m_DiskSpawnTimer;
	
	std::vector<Qube*> m_pQubes;

	EnemyManager* m_pEnemyManager;
	
	void Initialize() override;
	
	void CreateConnections();
	void CreateEscheresqueRightConnections();
	void CreateEscheresqueLeftConnections();
	
	bool IsOutsideOfPyramid(Qube* pQube) const;
	bool IsTop(Qube* pQube) const;
	
	unsigned int FindOutsideQubeIndex() const;
	int GetQBertIndex() const;
	int GetIndex(Qube* pQube) const;
	
	void DiskSpawnerTimer();

};