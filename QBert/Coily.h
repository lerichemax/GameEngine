#pragma once
#include "Enemy.h"

class CoilyCharacterController;
class PyramidSystem;
class QubeSystem;
class Coily final : public Enemy
{
public:
	explicit Coily(unsigned int fallSoundId);
	
	Coily(Coily&& other) = delete;
	Coily& operator=(Coily const& rhs) = delete;
	Coily& operator=(Coily&& rhs) = delete;
	~Coily() = default;
	
	void Transform(bool isTransformed);
	bool IsTransformed() const { return m_bIsTransformed; }
	void FallSound() const;

protected:
	void Initialize() ;
	
	void SetDirectionTextures(ConnectionDirection dir) override;
	void MoveToCurrentQube() override;
	void DoDie() override;
	void DoMove(ConnectionDirection direction) override;
	void MeetCharacter(Characters* pOther) override;

private:
	bool m_bIsTransformed;
	CoilyCharacterController* m_pController;
	
	unsigned int m_FallSoundId;
	
	

	Coily(Coily const& other);
};