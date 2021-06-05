#pragma once
#include "Character.h"

class CharacterLives;
class CharacterPoint;
class QBert final : public Character
{
public:
	explicit QBert(unsigned int jumpId, unsigned int fallId, unsigned int swearId);
	
	QBert(QBert&& other) = delete;
	QBert& operator=(QBert const& rhs) = delete;
	QBert& operator=(QBert&& rhs) = delete;
	~QBert() = default;

	int GetPlayerNumber() const { return m_PlayerNbr; }
	
	void EarnPoints(int points);
	void JumpOffDisk();
	void Reset(bool fullReset, Qube* pTargetQube);
	void SetCanMove() { m_bCanMove = true; }
	void SetPlayerNbr(int nbr) { m_PlayerNbr = nbr; }
	int GetPlayerNbr() const { return m_PlayerNbr; }
	void Swear()const;

protected:
	QBert* Clone() const override { return new QBert(*this); }
	
	void MeetCharacter(Character* pOther) override;
	void LandOnQube() override;
	void DoDie() override;
	void DoMove(ConnectionDirection direction) override;
	void Initialize() override;
	void Update() override;
	void SetDirectionTextures(ConnectionDirection dir) override;

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

	QBert(QBert const& other);
};
