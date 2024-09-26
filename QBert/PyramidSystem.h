#pragma once
#include "System.h"
#include "Event.h"

enum class Level;
enum class ConnectionDirection;
enum class EnemyType;
enum class MovementMode;

class EnemySpawnerSystem;
class QubeSystem;
class DiskSystem;
class ComponentManager;
struct PyramidComponent;
class PyramidSystem final : public System
{
public:
	PyramidSystem() = default;
	~PyramidSystem();

	EventHandler<PyramidSystem, int> OnAllQubesFlipped{};
	
	Entity GetTop() const; 
	Entity GetRandomTopQube() const;
	Entity GetSpawnQube(MovementMode mode) const;
	int GetQubeIndex(Entity qubeEntity) const;
	
	void Reset(Level level);
	void PartialReset();
	bool TryFindPathTo(Entity startingQube, Entity targetQube, ConnectionDirection* directionsArray, size_t arraySize) const; //used by coily to move to qbert

	void Serialize(StreamWriter& writer) const override;
	void SetSignature() const override;

protected:
	void Initialize();
	void Start();
	void Update();

private:
	DiskSystem* m_pDiskSystem;

	void CreateConnections(std::vector<Entity> const& qubes);
	void CreateEscheresqueRightConnections(PyramidComponent* const pPyramid);
	void CreateEscheresqueLeftConnections(PyramidComponent* const pPyramid);
	
	bool IsOutsideOfPyramid(Entity QubeEntity, PyramidComponent* const pPyramid) const;
	
	unsigned int FindOutsideQubeIndex(PyramidComponent* const pPyramid) const;

	Entity GetEscheresqueLeftTop() const;
	Entity GetEscheresqueRightTop() const;
	
	void DiskSpawnerTimer();
	void CheckAllQubesFlipped() const;
};