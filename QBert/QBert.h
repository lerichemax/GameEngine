#pragma once
#include "Character.h"

class QBert final : public Character
{
public:
	QBert();
	//QBert(Qube* pCurrentCube);
	QBert(QBert const& other) = delete;
	QBert(QBert&& other) = delete;
	QBert& operator=(QBert const& rhs) = delete;
	QBert& operator=(QBert&& rhs) = delete;
	~QBert();

	void Update() override { Character::Update(); }

	int GetPoints() const { return m_NbrPoints; }
	int GetLives() const { return m_NbrLives; }
	int GetPlayerNumber() const { return m_PlayerNbr; }
	
	void Die() override;
	void EarnPoints(int points);
	void Move(ConnectionDirection direction) override;
	void JumpOffDisk();
	void Reset(bool fullReset, Qube* pTargetQube);
	void SetCanMove() { m_bCanMove = true; }
protected:
	void MeetCharacter(Character* pOther) override;
	void LandOnQube() override;

private:
	int const MAX_LIVES{ 3 };
	
	static int m_PlayerNbr;
	int m_NbrLives;
	int m_NbrPoints;

	bool m_bCanMove;

	void Initialize() override;
	void SetDirectionTextures(ConnectionDirection dir) override;
};
