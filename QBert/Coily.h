#pragma once
#include "Enemy.h"

class CoilyCharacterController;
class Pyramid;
class Qube;
class Coily final : public Enemy
{
public:
	Coily();
	Coily* Clone() override { return new Coily(*this); }
	Coily(Coily&& other) = delete;
	Coily& operator=(Coily const& rhs) = delete;
	Coily& operator=(Coily&& rhs) = delete;
	~Coily() = default;
	
	void Move(ConnectionDirection direction) override;
	void MeetCharacter(Character* pOther) override;
	void Die() override;
	
	void Transform(bool isTransformed);
	
	void SetCurrentQube(Qube* pQube) { m_pCurrentQube = pQube; }
	
	bool IsTransformed() const { return m_bIsTransformed; }

protected:
	void SetDirectionTextures(ConnectionDirection dir) override;
	void MoveToCurrentQube() override;

private:
	bool m_bIsTransformed;
	CoilyCharacterController* m_pController;
	
	
	void Initialize() override;

	Coily(Coily const& other);
};