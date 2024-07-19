#pragma once
#include "BehaviourComponent.h"

#include <vector>
#include <glm/glm.hpp>

class EnemyManager_bu;
class Qube;
enum class ConnectionDirection;
class QBert;
class ColoredDisk;
class Pyramid final : public BehaviourComponent
{

public:
	explicit Pyramid(unsigned int maxWidth);
	Pyramid() = default;
	
	~Pyramid();

	void Update() override;
	
	std::shared_ptr<Qube> GetTop() const { return m_pQubes.front(); }
	std::shared_ptr<Qube> GetEscheresqueLeftTop() const { return m_pQubes.back(); }
	std::shared_ptr<Qube> GetEscheresqueRightTop() const { return m_pQubes[m_pQubes.size() - m_MaxWidth]; }
	std::shared_ptr<Qube> GetQube(int index) const { return m_pQubes[index]; }
	int GetQubeIndex(std::shared_ptr<Qube> pQube) const;
	int GetNbrDisks()const { return m_NbrDisksSpawned; }
	std::vector<std::shared_ptr<Qube>> const& GetQubes() const { return m_pQubes; }
	
	bool AreAllQubesFlipped() const;
	void Reset();
	void PartialReset();
	void DiskUsed(){ m_NbrDisksSpawned--; }
	void SetQBert(QBert* pQbert) { m_pQBert = pQbert; }
	bool FindNextQubeToQbert(std::shared_ptr<Qube> const pStartingQube, ConnectionDirection* directions, unsigned int size) const; //used by coily to move to qbert
	QBert* GetQBert()const { return m_pQBert; }

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

private:
	unsigned int m_MaxWidth;
	unsigned int const MAX_NBR_DISKS{ 4 };
	float const DISK_SPAWNING_INTERVAL{ 7.f };
	
	unsigned int m_NbrDisksSpawned;
	
	float m_DiskSpawnTimer;
	
	std::vector<std::shared_ptr<Qube>> m_pQubes;
	QBert* m_pQBert;
	
	void Initialize() override;
	
	void CreateConnections();
	void CreateEscheresqueRightConnections();
	void CreateEscheresqueLeftConnections();
	
	bool IsOutsideOfPyramid(std::shared_ptr<Qube> pQube) const;
	bool IsTop(std::shared_ptr<Qube> pQube) const;
	
	unsigned int FindOutsideQubeIndex() const;
	int GetQBertIndex() const;
	
	void DiskSpawnerTimer();
};