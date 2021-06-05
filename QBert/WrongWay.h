#pragma once
#include "Enemy.h"

class WrongWay final : public Enemy
{
public:
	WrongWay(bool escheresqueRight);

	WrongWay* Clone() const override { return new WrongWay(*this); }

	void Initialize() override;
	void Move(ConnectionDirection direction) override;
	void MoveToCurrentQube() override{};
	void JumpToQube(Qube* pNextQube) override;
	virtual void Die() override;
	
	void SetCurrentQube(Qube* pQube) { m_pCurrentQube = pQube; }
	void SetEscheresqueRight(bool isRight) { m_bEscheresqueRight = isRight; }
	
	bool IsEscheresqueRight() const { return m_bEscheresqueRight; }

protected:
	void SetDirectionTextures(ConnectionDirection dir) override;
	void SetLayerToBackGround() override{};
private:
	bool m_bEscheresqueRight;

};