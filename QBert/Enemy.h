#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(int pointsForKill, CharacterType type);
	 ~Enemy() override  = default;

	int GetPointsForKill()const { return POINTS_FOR_KILL; }
protected:
	friend class EnemyManager;
	
	unsigned int const POINTS_FOR_KILL;

	
	void MeetCharacter(Character* ) override {}
	void SetDirectionTextures(ConnectionDirection) override {}
	void DoMove(ConnectionDirection direction) override;
	void DoDie() override;

	Enemy* Clone() const override { return new Enemy(*this); }
	
	Enemy(Enemy const& other);

};