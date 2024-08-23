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
	~PyramidSystem() = default;

	EventHandler<PyramidSystem, int> OnAllQubesFlipped{};
	
	//QubeSystem* GetTop() const { return m_pQubes.front(); }
	//QubeSystem* GetEscheresqueLeftTop() const { return m_pQubes.back(); }
	//QubeSystem* GetEscheresqueRightTop() const { return m_pQubes[m_pQubes.size() - MAX_WIDTH]; }
	//QubeSystem* GetQube(int index) const { return m_pQubes[index]; }
	int GetQubeIndex(QubeSystem* const pQube) const;
	//int GetNbrDisks()const { return m_NbrDisksSpawned; }
	//std::vector<QubeSystem*> const& GetQubes() const { return m_pQubes; }
	
	void Reset(Level level);
	void PartialReset();
	//void DiskUsed(){ m_NbrDisksSpawned--; }
	//void SetQBert(QBert* pQbert) { m_pQBert = pQbert; }
	bool FindNextQubeToQbert(QubeSystem* const pStartingQube, ConnectionDirection* directions, unsigned int size) const; //used by coily to move to qbert
	//QBert* GetQBert()const { return m_pQBert; }

	void Serialize(StreamWriter& writer) const override;
	void SetSignature(Coordinator* const pRegistry) override;

protected:
	void Update(ComponentManager* const pCompManager);

private:
	void Initialize(ComponentManager* const pCompManager);
	
	void CreateConnections(std::vector<Entity> const& qubes, ComponentManager* const pCompManager);
	void CreateEscheresqueRightConnections();
	void CreateEscheresqueLeftConnections();
	
	bool IsOutsideOfPyramid(QubeSystem* const pQube) const;
	bool IsTop(QubeSystem* const pQube) const;
	
	unsigned int FindOutsideQubeIndex() const;
	int GetQBertIndex() const;
	
	void DiskSpawnerTimer();
	void CheckAllQubesFlipped(Entity entity, ComponentManager* const pCompManager) const;
};