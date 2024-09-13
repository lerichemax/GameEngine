#pragma once
#include "System.h"

class Coordinator;
class QBertSystem : public System 
{
public:
	 void SetSignature() override;

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
};