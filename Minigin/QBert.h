#pragma once
#include "GameObject.h"
#include "UIObject.h"

class QBert final : public dae::GameObject
{
public:
	QBert(dae::Tag tag);
	void Die();
	void EarnPoints();
	int GetPoints() const { return m_NbrPoints; }
private:
	int m_NbrLives;
	int m_NbrPoints;

};