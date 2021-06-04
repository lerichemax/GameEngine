#pragma once
#include "Enemy.h"

class Qube;
class SlickSam final : public Enemy
{
public:
	SlickSam();
	SlickSam* Clone() const override { return new SlickSam(*this); }
	~SlickSam() override;
	
	void Die() override;

protected:
	void MeetCharacter(Character* pOther) override;

private:
	int static m_NbrInstances;

	void Initialize() override;
	void SetTexture();

	SlickSam(SlickSam const& other);
};