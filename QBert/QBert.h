#pragma once
#include "Component.h"

namespace empire {
	class Subject;
}

class QBert final : public empire::Component
{
public:
	QBert();
	QBert(QBert const& other) = delete;
	QBert(QBert&& other) = delete;
	QBert& operator=(QBert const& rhs) = delete;
	QBert& operator=(QBert&& rhs) = delete;
	~QBert();
	
	void Update() override {}
	
	void Die();
	void EarnPoints();
	int GetPoints() const { return m_NbrPoints; }
	int GetLives() const { return m_NbrLives; }
	empire::Subject* const GetSubject() const { return m_pSubject; }
	int GetPlayerNumber() const { return m_PlayerNbr; }
private:
	static int m_PlayerNbr;
	int m_NbrLives;
	int m_NbrPoints;

	empire::Subject* const m_pSubject;
};
