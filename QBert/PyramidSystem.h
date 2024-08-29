#pragma once
#include "System.h"
#include "Event.h"

#include <vector>
#include <glm/glm.hpp>

class EnemyManager;
class QubeSystem;
enum class Level;
enum class ConnectionDirection;
class QBert;
class ColoredDisk;
class ComponentManager;
class PyramidSystem final : public System
{
public:
	PyramidSystem() = default;
	~PyramidSystem();

	EventHandler<PyramidSystem, int> OnAllQubesFlipped{};
	
	Entity GetTop() const; 
	//QubeSystem* GetEscheresqueLeftTop() const { return m_pQubes.back(); }
	//QubeSystem* GetEscheresqueRightTop() const { return m_pQubes[m_pQubes.size() - MAX_WIDTH]; }
	//QubeSystem* GetQube(int index) const { return m_pQubes[index]; }
	int GetQubeIndex(QubeSystem* const pQube) const;
	//int GetNbrDisks()const { return NbrDisksSpawned; }
	//std::vector<QubeSystem*> const& GetQubes() const { return m_pQubes; }
	
	void Reset(Level level);
	void PartialReset();
	//void DiskUsed(){ NbrDisksSpawned--; }
	//void SetQBert(QBert* pQbert) { m_pQBert = pQbert; }
	bool FindNextQubeToQbert(QubeSystem* const pStartingQube, ConnectionDirection* directions, unsigned int size) const; //used by coily to move to qbert
	//QBert* GetQBert()const { return m_pQBert; }

	void Serialize(StreamWriter& writer) const override;
	void SetSignature() override;

protected:
	void Initialize();
	void Start();
	void Update();

private:
	void CreateConnections(std::vector<Entity> const& qubes);
	void CreateEscheresqueRightConnections();
	void CreateEscheresqueLeftConnections();
	
	bool IsOutsideOfPyramid(QubeSystem* const pQube) const;
	bool IsTop(QubeSystem* const pQube) const;
	
	unsigned int FindOutsideQubeIndex() const;
	int GetQBertIndex() const;
	
	void DiskSpawnerTimer();
	void CheckAllQubesFlipped() const;
};