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

	void Serialize(StreamWriter& writer) const;
	void Deserialize(JsonReader* const reader, SerializationMap& context);

private:
	int m_NbrPoints;
};
