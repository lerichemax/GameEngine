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
	void SetDirectionTextures(ConnectionDirection dir) override;
private:
	int static m_NbrInstances;

	bool m_bIsSam;
	
	void Initialize() override;
	void SetTexture();

	SlickSam(SlickSam const& other);
};