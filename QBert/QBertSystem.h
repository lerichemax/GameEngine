#pragma once
#include "System.h"
#include "Event.h"

enum class GameMode;
class QBertSystem : public ecs::System
{
public:
	EventHandler<QBertSystem, Entity> OnQBertEncounteredEnemy;

	 void ResetToCurrentQube(Entity qbertEntity);
	 void Reset(GameMode gameMode);

	 void SetStartQubes(GameMode mode);
	 void AddPoints(int points);

protected:
	void Start() override;
	void Update() override;

private:
	void OnJumped(Entity qbertEntity);
	void HandleEnemyEncounter(Entity characterEntity, std::unordered_set<Entity> const& qubeCharacterEntities);
	void OnJumpedToDeath(Entity qbertEntity);
	void OnFell(Entity qbertEntity);
	void JumpOffDisk(Entity qbertEntity);
	void GetHurt(Entity qbertEntity);

	void SetToQube(Entity qbertEntity, Entity qubeEntity);
};