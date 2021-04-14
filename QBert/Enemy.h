#pragma once
#include "Character.h"
#include "EnemyObserver.h"

class Enemy : public Character
{
public:
	Enemy(Qube* pQube, int pointsForKill);
	virtual ~Enemy() override  = default;
	
	virtual void Update() override;
	virtual void Move(ConnectionDirection direction) override;
	virtual void Die() override;

	int GetPointsForKill()const { return POINTS_FOR_KILL; }
protected:
	unsigned int const POINTS_FOR_KILL;
	
	virtual ConnectionDirection ChooseDirection();

	virtual void MeetCharacter(Character* pOther) override {}
	void LandOnQube() override{}
	void SetDirectionTextures(ConnectionDirection dir) override {}

private:
	float const MOVE_MAX_TIME{ 1.f };
	
	
	float m_MoveTimer;

};