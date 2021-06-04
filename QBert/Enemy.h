#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(int pointsForKill, CharacterType type);
	virtual ~Enemy() override  = default;

	virtual Enemy* Clone() const override { return new Enemy(*this); }
	
	virtual void Move(ConnectionDirection direction) override;
	virtual void Die() override;
	void Initialize() override
	{
		Character::Initialize();
	}
	void Update() override
	{
		Character::Update();
	}
	int GetPointsForKill()const { return POINTS_FOR_KILL; }
protected:
	friend class EnemyManager_bu;
	
	unsigned int const POINTS_FOR_KILL;

	
	virtual void MeetCharacter(Character* ) override {}
	void SetDirectionTextures(ConnectionDirection) override {}

	
	Enemy(Enemy const& other)
		:Character(other),
		POINTS_FOR_KILL(other.POINTS_FOR_KILL)
	{}
};