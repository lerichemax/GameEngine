#pragma once
#include "System.h"

#include "Event.h"

class ComponentManager;
struct JumpComponent;
class ECS_TransformComponent;
class JumperSystem : public System
{
public:
	JumperSystem() = default;

	EventHandler<JumperSystem> OnJumpLanded;
	EventHandler<JumperSystem> OnFell;
	EventHandler<JumperSystem> OnJumpedToDeath;

	virtual void Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos, JumpComponent* const pJump);
	void JumpToDeath(glm::vec2 const& startPos, float xDist, JumpComponent* const pJump);

	void Serialize(StreamWriter& writer) const override;

	void SetSignature(Coordinator* const pRegistry) override;

protected:
	void Update(ComponentManager* const pComponentManager) override;

private:
	void UpdateJump(JumpComponent* const pJump, ECS_TransformComponent* const pTransform);
	virtual void UpdateFall(JumpComponent* const pJump, ECS_TransformComponent* const pTransform);
};
