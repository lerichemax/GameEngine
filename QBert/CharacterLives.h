#pragma once
#include "Component.h"
#include "Event.h"

struct CharacterLives final : public Component
{
public:
	CharacterLives();

	EventHandler<CharacterLives, int> OnDied;
	EventHandler<CharacterLives> OnGameOver;

	void Die();
	int GetNbrLives() const { return m_NbrLives; }
	bool IsGameOver() const { return m_NbrLives == 0; }
	void Reset();

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

private:
	int const MAX_LIVES{ 3 };

	int m_NbrLives;

};