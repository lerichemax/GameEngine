#pragma once
#include "Character.h"

class Enemy : public Characters
{
public:
	explicit Enemy(int pointsForKill, CharacterType type);

	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy const& rhs) = delete;
	Enemy& operator=(Enemy&& rhs) = delete;
	
	 ~Enemy() override  = default;
	 
	
	int GetPointsForKill()const { return POINTS_FOR_KILL; }
protected:
	friend class EnemySpawnerSystem;
	
	unsigned int const POINTS_FOR_KILL;

	
	void MeetCharacter(Characters* ) override {}
	void SetDirectionTextures(ConnectionDirection) override {}
	void DoMove(ConnectionDirection direction) override;
	void DoDie() override;

	Enemy(Enemy const& other);

};