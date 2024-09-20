#pragma once
#include "System.h"

#include "Event.h"

class TextureRendererComponent;
class JumperSystem;
struct MovementComponent;
class CharacterMovementSystem : public System
{
public:
	EventHandler<CharacterMovementSystem, Entity> OnMoveStarted;
	EventHandler<CharacterMovementSystem, Entity> OnJumpedOnDisk;
	EventHandler<CharacterMovementSystem, Entity, std::unordered_set<Entity> const&> OnMeetCharacter;

	void SetSignature() const override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Start();
	void Update() override;

private:
	void SetIdleTexture(Entity entity);
	void SetJumpTexture(Entity entity);
	void MoveToCurrentQube(Entity entity);
	void Move(Entity entity);

	JumperSystem* m_pJumper;
};
