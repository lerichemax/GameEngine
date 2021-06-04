#pragma once
#include "Enemy.h"

class CoilyCharacterController;
class Pyramid;
class Qube;
class Coily final : public Enemy
{
public:
	Coily(unsigned int fallSoundId);
	Coily* Clone() const override { return new Coily(*this); }
	Coily(Coily&& other) = delete;
	Coily& operator=(Coily const& rhs) = delete;
	Coily& operator=(Coily&& rhs) = delete;
	~Coily() = default;
	
	void Move(ConnectionDirection direction) override;
	void MeetCharacter(Character* pOther) override;
	void Die() override;
	
	void Transform(bool isTransformed);
	
	bool IsTransformed() const { return m_bIsTransformed; }
	void FallSound() const;
protected:
	void SetDirectionTextures(ConnectionDirection dir) override;
	void MoveToCurrentQube() override;

private:
	bool m_bIsTransformed;
	CoilyCharacterController* m_pController;
	
	unsigned int m_FallSoundId;
	
	void Initialize() override;

	Coily(Coily const& other);
};