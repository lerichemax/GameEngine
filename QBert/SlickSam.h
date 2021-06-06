#pragma once
#include "Enemy.h"

class Qube;
class SlickSam final : public Enemy
{
public:
	explicit SlickSam();
	SlickSam* Clone() const override { return new SlickSam(*this); }

	SlickSam(SlickSam&& other) = delete;
	SlickSam& operator=(SlickSam const&) = delete;
	SlickSam& operator=(SlickSam&&) = delete;
	
	~SlickSam() override;
	
protected:
	void MeetCharacter(Character* pOther) override;
	void DoDie() override;
	void SetDirectionTextures(ConnectionDirection dir) override;

private:
	int static m_NbrInstances;

	bool m_bIsSam;
	
	void Initialize() override;
	void SetTexture();

	SlickSam(SlickSam const& other);
};