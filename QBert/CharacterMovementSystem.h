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

	void Move(Entity entity);
	void MoveToCurrentQube(Entity entity);
	//Qube* const GetCurrentQube() const;

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Start();
	void Update() override;

private:
	void JumpToCurrentQube(Entity entity);
	void SetIdleTexture(Entity entity);
	void SetJumpTexture(Entity entity);

	JumperSystem* m_pJumper;
};
