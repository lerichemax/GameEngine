#pragma once
#include "Component.h"

#include <vector>
#include <glm/glm.hpp>

using namespace empire;

class EnemyManager_bu;
class Qube;
enum class ConnectionDirection;
class QBert;
class ColoredDisk;
class Pyramid final : public Component
{

public:
	Pyramid(unsigned int maxWidth);
	
	Pyramid* Clone() override { return new Pyramid(*this); } 
	
	Pyramid(Pyramid&& other) = delete;
	Pyramid& operator=(Pyramid const& rhs) = delete;
	Pyramid& operator=(Pyramid&& rhs) = delete;
	~Pyramid();

	void Update() override;
	
	Qube* GetTop() const { return m_pQubes.front(); }
	Qube* GetEscheresqueLeftTop() const { return m_pQubes.back(); }
	Qube* GetEscheresqueRightTop() const { return m_pQubes[m_pQubes.size() - MAX_WIDTH]; }
	Qube* GetQube(int index) const { return m_pQubes[index]; }
	int GetQubeIndex(Qube* pQube) const;
	
	std::vector<Qube*> const& GetQubes() const { return m_pQubes; }
	
	bool AreAllQubesFlipped() const;
	void Reset();
	void PartialReset();
	void DiskUsed(){ m_NbrDisksSpawned--; }
	void SetQBert(QBert* pQbert) { m_pQBert = pQbert; }
	bool FindNextQubeToQbert(Qube* const pStartingQube, ConnectionDirection* directions, const int size) const; //used by coily to move to qbert
	QBert* GetQBert()const { return m_pQBert; }

private:
	unsigned int const MAX_WIDTH;
	unsigned int const MAX_NBR_DISKS{ 4 };
	float const DISK_SPAWNING_INTERVAL{ 7.f };
	
	unsigned int m_NbrDisksSpawned;
	
	float m_DiskSpawnTimer;
	
	std::vector<Qube*> m_pQubes;
	QBert* m_pQBert;
	
	void Initialize() override;
	
	void CreateConnections();
	void CreateEscheresqueRightConnections();
	void CreateEscheresqueLeftConnections();
	
	bool IsOutsideOfPyramid(Qube* pQube) const;
	bool IsTop(Qube* pQube) const;
	
	unsigned int FindOutsideQubeIndex() const;
	int GetQBertIndex() const;
	
	void DiskSpawnerTimer();

	Pyramid(Pyramid const& other);
};