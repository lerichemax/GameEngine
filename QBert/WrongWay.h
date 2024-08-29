#pragma once
#include "Enemy.h"

class WrongWay final : public Enemy
{
public:
	explicit WrongWay(bool escheresqueRight);

	WrongWay(WrongWay&& other) = delete;
	WrongWay & operator=(WrongWay const&) = delete;
	WrongWay& operator=(WrongWay&&) = delete;
	~WrongWay() = default;
	
	void Initialize() ;
	void DoMove(ConnectionDirection direction) override;
	void MoveToCurrentQube() override{};
	void JumpToQube(QubeSystem* const pNextQube) override;
	virtual void DoDie() override;
	
	void SetCurrentQube(QubeSystem* const pQube) { m_pCurrentQube = pQube; }
	void SetEscheresqueRight(bool isRight) { m_bEscheresqueRight = isRight; }
	
	bool IsEscheresqueRight() const { return m_bEscheresqueRight; }

protected:
	void SetDirectionTextures(ConnectionDirection dir) override;
	void SetLayerToBackGround() override{};

private:
	bool m_bEscheresqueRight;

	WrongWay(WrongWay const& other);

};