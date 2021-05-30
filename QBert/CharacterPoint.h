#pragma once
#include "Component.h"

class CharacterPoint final : public empire::Component
{
public:
	CharacterPoint();
	CharacterPoint* Clone() { return new CharacterPoint(*this); }
	void Initialize() override{};
	void Update() override{};
	
	void AddPoints(int nbrPoint) { m_NbrPoints += nbrPoint; }
	int GetPoints() { return m_NbrPoints; }
	void Reset() { m_NbrPoints = 0; }
private:

	int m_NbrPoints;
};