#pragma once
#include "Enemy.h"

class Qube;
class SlickSam final : public Enemy
{
public:
	SlickSam(Qube* pQube);
	~SlickSam() override;
	
	void Die() override;

protected:
	void MeetCharacter(Character* pOther) override;
private:
	int static m_NbrInstances;

	void Initialize() override;
	void SetTexture();
};