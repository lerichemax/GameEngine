#pragma once
#include "System.h"

#include "Event.h"

class JumperSystem : public ecs::System
{
public:
	JumperSystem() = default;

	EventHandler<JumperSystem, Entity> OnJumpLanded;
	EventHandler<JumperSystem, Entity> OnFell;
	EventHandler<JumperSystem, Entity> OnJumpedToDeath;

	virtual void Jump(Entity entity, glm::vec2 const& startPos, glm::vec2 const& targetPos);
	void JumpToDeath(Entity entity, glm::vec2 const& startPos, float xDist);

protected:
	void Update() override;

private:
	void UpdateJump(Entity entity);
	virtual void UpdateFall(Entity entity);
};
