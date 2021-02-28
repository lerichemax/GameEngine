#pragma once
#include "GameObject.h"

class QBert final : public dae::GameObject
{
public:
	QBert();
	void Die();
private:
	int m_NbrLives;
};