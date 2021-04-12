#pragma once
#include "Enemy.h"

class Pyramid;
class Coily final : public Enemy
{
public:
	Coily(Pyramid* pPyramid);

	void Update() override;
	void Transform();

	void SetIsIdle(bool isIdle) { m_bIsIdle = isIdle; }
protected:
	ConnectionDirection ChooseDirection() const override;

private:
	bool m_bIsTransformed{ false };
	bool m_bIsIdle{ false };
	
	Pyramid* m_pPyramid;
};