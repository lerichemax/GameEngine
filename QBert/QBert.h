#pragma once
#include "Character.h"

class CharacterLives;
class CharacterPoint;
class QBert final : public Character
{
public:
	QBert(unsigned int jumpId, unsigned int fallId, unsigned int swearId);
	QBert* Clone() override { return new QBert(*this); }
	
	QBert(QBert&& other) = delete;
	QBert& operator=(QBert const& rhs) = delete;
	QBert& operator=(QBert&& rhs) = delete;
	~QBert() = default;

	int GetPlayerNumber() const { return m_PlayerNbr; }
	void Die() override;
	void EarnPoints(int points);
	void Move(ConnectionDirection direction) override;
	void JumpOffDisk();
	void Reset(bool fullReset, Qube* pTargetQube);
	void SetCanMove() { m_bCanMove = true; }
	void SetPlayerNbr(int nbr) { m_PlayerNbr = nbr; }
	int GetPlayerNbr() const { return m_PlayerNbr; }
	void Swear()const;
protected:
	void MeetCharacter(Character* pOther) override;
	void LandOnQube() override;

private:
	CharacterPoint* m_pPoints;
	CharacterLives* m_pLives;
	RendererComponent* m_pHurtTex;

	unsigned int m_JumpSoundID;
	unsigned int m_FallSoundID;
	unsigned int m_SwearSoundID;
	
	int m_PlayerNbr;

	bool m_bCanMove;
	bool m_bWillSleep;
	
	void Initialize() override;
	void Update() override;
	void SetDirectionTextures(ConnectionDirection dir) override;

	QBert(QBert const& other);
};
