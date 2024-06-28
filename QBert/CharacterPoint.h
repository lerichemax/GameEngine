#pragma once
#include "Component.h"

class CharacterPoint final : public Component
{
public:
	CharacterPoint();
	CharacterPoint* Clone() const override { return new CharacterPoint(*this); }
	void Initialize() override{};
	void Update() override{};
	
	void AddPoints(int nbrPoint) { m_NbrPoints += nbrPoint; }
	int GetPoints() const { return m_NbrPoints; }
	void Reset() { m_NbrPoints = 0; }
private:

	int m_NbrPoints;
};

class ECS_CharacterPoint final : public ECS_Component
{
public:
	ECS_CharacterPoint();

	void AddPoints(int nbrPoint) { m_NbrPoints += nbrPoint; }
	int GetPoints() const { return m_NbrPoints; }
	void Reset() { m_NbrPoints = 0; }

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

private:
	int m_NbrPoints;
};
