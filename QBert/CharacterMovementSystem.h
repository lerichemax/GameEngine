#pragma once
#include "System.h"

#include "Event.h"

class TextureRendererComponent;
class JumperSystem;
struct MovementComponent;
class CharacterMovementSystem : public System
{
public:
	EventHandler<CharacterMovementSystem> OnMoveStarted;

	void Move(TextureRendererComponent* const pRenderer, MovementComponent* const pMoveComp);
	//void SetCurrentQube(Qube* const pQube);
	//Qube* const GetCurrentQube() const;

	void SetSignature(Coordinator* const pRegistry) override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Start();
	void Update(ComponentManager* const pComponentManager) override;

private:
	void JumpToCurrentQube();
	void MoveToCurrentQube();
	void SetIdleTexture(TextureRendererComponent* const pRenderer, MovementComponent* const pMoveComp);
	void SetJumpTexture(TextureRendererComponent* const pRenderer, MovementComponent* const pMoveComp);

	JumperSystem* m_pJumper;
};
