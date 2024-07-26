#pragma once
#include "Jumper.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>

class WrongWayJumper final : public Jumper
{
public:
	WrongWayJumper();

	void Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos) override;

	void UpdateFall() override;

};
