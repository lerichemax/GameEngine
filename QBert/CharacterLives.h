#pragma once
#include "Component.h"

class CharacterLives final : public Component
{
public:
	explicit CharacterLives(int maxLives);
	CharacterLives* Clone() const override { return new CharacterLives(*this); }
	
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

struct ECS_CharacterLives final : public ECS_Component
{
public:
	explicit ECS_CharacterLives(int maxLives);
	ECS_CharacterLives() = default;

	void Die();
	int GetNbrLives() const { return m_NbrLives; }
	bool IsGameOver() const { return m_NbrLives == 0; }
	void Reset();

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
private:
	int m_MaxLives;

	int m_NbrLives;

};