#pragma once
#include "GameObject.h"
#include "UIObject.h"

class QBert final : public dae::GameObject
{
public:
	QBert(dae::UIObject* plivesCounter);
	void Die();

private:
	int m_NbrLives;
	dae::UIObject* m_pScreenCounter;
	
};