#pragma once
#include "Component.h"
#include "Event.h"

class CharacterPoint final : public ecs::Component
{
public:
	CharacterPoint();

	EventHandler<CharacterPoint, int, Entity> OnPointsUpdated;

	void AddPoints(int nbrPoint);
	int GetPoints() const { return m_NbrPoints; }
	void Reset() { m_NbrPoints = 0; }


private:
	PROPERTY(int, m_NbrPoints);
};

SERIALIZE_CLASS(CharacterPoint, ecs::Component)