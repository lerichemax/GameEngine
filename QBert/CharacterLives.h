#pragma once
#include "Component.h"

struct CharacterLives final : public Component
{
	friend class LivesSystem;

public:
	CharacterLives();

	PROPERTY(int, MaxLives);
	PROPERTY(int, NbrLives);

	void Init(int nbrLives);
	void Die();
	void Reset();

private:
	bool bIsDead{false};
};

SERIALIZE_CLASS(CharacterLives, ecs::Component)