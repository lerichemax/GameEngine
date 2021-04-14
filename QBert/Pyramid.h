#pragma once
#include "Component.h"

#include <vector>
#include <glm/glm.hpp>

#include <array>
using namespace empire;

class Qube;
class Coily;
enum class ConnectionDirection;
class QBert;
class ColoredDisk;
class Pyramid final : public Component
{
	//CONSTANTS
	unsigned int const MAX_WIDTH;
	unsigned int const MAX_NBR_DISKS{ 4 };
	float const DISK_SPAWNING_INTERVAL{ 20.f };
	unsigned int static const MAX_COILY{ 1 };
	float const COILY_SPAWN_INTERVAL{ 15.f };
	unsigned int const MAX_SLICKSAM{ 2 }; //No more than 2
	float const SLICKSAM_SPAWN_INTERVAL{ 10.f };

public:
	Pyramid(unsigned int maxWidth, QBert* pQbert);
	Pyramid(Pyramid const& other) = delete;
	Pyramid(Pyramid&& other) = delete;
	Pyramid& operator=(Pyramid const& rhs) = delete;
	Pyramid& operator=(Pyramid&& rhs) = delete;
	~Pyramid();

	void Update() override;
	
	Qube* GetTop() const { return m_pQubes.front(); }
	Qube* GetQube(int index) const { return m_pQubes[index]; }
	std::array<Coily*, MAX_COILY> const& GetCoilies() const { return m_pCoilies; }
	
	bool AreAllQubesFlipped() const;
	void Reset();
	void DiskUsed() { m_NbrDisksSpawned--; }
	void CoilyDied(Coily* pCoily);
	void SlickSamDied() { m_NbrSlickSam--; }
	void FindNextQubeToQbert(Qube* const pStartingQube, ConnectionDirection* directions, const int size) const; //used by coily to move to qbert

private:
	unsigned int m_NbrDisksSpawned;
	unsigned int m_NbrSlickSam;
	unsigned int m_NbrCoily;
	
	float m_DiskSpawnTimer;
	float m_CoilySpawnTimer;
	float m_SlickSamSpawnTimer;
	
	std::vector<Qube*> m_pQubes;
	std::array<Coily*, MAX_COILY> m_pCoilies;

	void Initialize() override;
	
	void CreateConnections();
	unsigned int FindOutsideQubeIndex() const;
	bool IsOutsideOfPyramid(Qube* pQube) const;
	bool IsTop(Qube* pQube) const;
	int GetQBertIndex() const;
	int GetIndex(Qube* pQube) const;
	void AddCoilyToArray(Coily* pCoily);
	
	void DiskSpawnerTimer();
	void CoilySpawnerTimer();
	void SlickSamSpawnerTimer();
};