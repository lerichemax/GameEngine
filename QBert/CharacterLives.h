#pragma once
#include "Component.h"

class CharacterLives final : public empire::Component
{
public:
	CharacterLives(int maxLives);
	CharacterLives* Clone() { return new CharacterLives(*this); }
	
	void Initialize() override{};
	void Update() override{};
	
	void Die();
	int GetNbrLives() const { return m_NbrLives; }
	bool IsGameOver() const { return m_bIsGameOver; }
	void Reset();
private:
	int const MAX_LIVES;

	int m_NbrLives;

	bool m_bIsGameOver;
};