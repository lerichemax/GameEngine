#pragma once
#include "Character.h"
#include "Qube.h"

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

	void Update() override {}

	int GetPoints() const { return m_NbrPoints; }
	int GetLives() const { return m_NbrLives; }
	int GetPlayerNumber() const { return m_PlayerNbr; }
	
	void Die() override;
	void EarnPoints(int points);
	void Move(ConnectionDirection direction) override;
	void JumpOffDisk();
	void Reset(bool fullReset, Qube* pTargetQube);
private:
	int const MAX_LIVES{ 3 };
	
	static int m_PlayerNbr;
	int m_NbrLives;
	int m_NbrPoints;
};
