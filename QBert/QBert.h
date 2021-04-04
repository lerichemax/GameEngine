#pragma once
#include "Component.h"

namespace empire {
	class Subject;
}
class Qube;
class QBert final : public empire::Component
{
public:
	QBert(); //temp for demo purpose
	QBert(Qube* pCurrentCube);
	QBert(QBert const& other) = delete;
	QBert(QBert&& other) = delete;
	QBert& operator=(QBert const& rhs) = delete;
	QBert& operator=(QBert&& rhs) = delete;
	~QBert();

	void Init(empire::GameObject* pParent) override;
	void Update() override {}
	
	void Die();
	void EarnPoints();
	int GetPoints() const { return m_NbrPoints; }
	int GetLives() const { return m_NbrLives; }
	empire::Subject* const GetSubject() const { return m_pSubject; }
	int GetPlayerNumber() const { return m_PlayerNbr; }
	Qube* const GetCurrentQube() const { return m_pCurrentQube; }
	void Move(Qube* pTargetQube);
private:
	static int m_PlayerNbr;
	int m_NbrLives;
	int m_NbrPoints;
	Qube* m_pCurrentQube;
	
	empire::Subject* const m_pSubject;
};
