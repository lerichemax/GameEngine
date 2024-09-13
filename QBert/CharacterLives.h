#pragma once
#include "Component.h"

struct CharacterLives final : public Component
{
	friend class LivesSystem;

public:
	CharacterLives();

	int MaxLives{};
	int NbrLives{};

	void Init(int nbrLives);
	void Die();
	void Reset();

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

private:
	bool bIsDead{false};
};