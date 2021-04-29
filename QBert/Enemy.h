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
	friend class EnemyManager;
	
	unsigned int const POINTS_FOR_KILL;
	
	virtual ConnectionDirection ChooseDirection();

	virtual void Initialize() override;
	virtual void MeetCharacter(Character* ) override {}
	//void LandOnQube() override{}
	void SetDirectionTextures(ConnectionDirection) override {}

private:
	float const MOVE_MAX_TIME{ 1.f };
	
	float m_MoveTimer;
};