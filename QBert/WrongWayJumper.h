#pragma once
#include "Jumper.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>

class WrongWayJumper final : public Jumper
{
public:
	WrongWayJumper();
	WrongWayJumper* Clone() const override { return new WrongWayJumper(*this); }

	void Jump(glm::vec2 const& startPos, glm::vec2 const& targetPos) override;

	void UpdateFall(empire::TransformComponent* transform) override;

};
