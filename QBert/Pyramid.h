#pragma once
#include "Component.h"

#include <vector>
#include <glm/glm.hpp>

using namespace empire;

class Qube;
class Coily;
enum class ConnectionDirection;
class QBert;
class ColoredDisk;
class Pyramid final : public Component
{
public:
	Pyramid(unsigned int maxWidth);
	Pyramid(Pyramid const& other) = delete;
	Pyramid(Pyramid&& other) = delete;
	Pyramid& operator=(Pyramid const& rhs) = delete;
	Pyramid& operator=(Pyramid&& rhs) = delete;
	~Pyramid();

	void InitializeQubes(QBert* pQbert);
	void Update() override;
	
	Qube* GetTop() const { return m_pQubes.front(); }
	Qube* GetQube(int index) const { return m_pQubes[index]; }
	std::vector<Coily*> const& GetCoilies() const { return m_pCoilies; }
	
	bool AreAllQubesFlipped() const;
	void Reset();
	void DiskUsed() { m_NbrDisksSpawned--; }
	void FindNextQubeToQbert(Qube* const pStartingQube, ConnectionDirection* directions, const int size) const; //used by coily to move to qbert

private:
	unsigned int const MAX_WIDTH;
	unsigned int const MAX_NBR_DISKS{ 3 };
	float const DISK_SPAWNING_INTERVAL{ 20.f };
	unsigned int const MAX_COLLY{ 1 };
	float const COILY_SPAWN_INTERVAL{ 15.f };

	unsigned int m_NbrDisksSpawned;

	float m_DiskSpawnTimer;
	float m_CoilySpawnTimer;
	
	std::vector<Qube*> m_pQubes;
	std::vector<Coily*> m_pCoilies;
	
	void CreateConnections();
	unsigned int FindOutsideQubeIndex() const;
	bool IsOutsideOfPyramid(Qube* pQube) const;
	bool IsTop(Qube* pQube) const;
	int GetQBertIndex() const;
	int GetIndex(Qube* pQube) const;
};