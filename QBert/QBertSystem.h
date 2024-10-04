#pragma once
#include "System.h"
#include "Event.h"

class Registry;
class QBertSystem : public System 
{
public:
	EventHandler<QBertSystem> OnQBertEncounteredEnemy;

	 void SetSignature() const override;

	 void Serialize(StreamWriter& writer) const override;

	 void Reset(bool fullReset, Entity targetQubeEntity);

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
};