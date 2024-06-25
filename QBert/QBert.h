#pragma once
#include "Character.h"


struct AudioComponent;
class CharacterLives;
class CharacterPoint;
class QBert final : public Character
{
public:
	explicit QBert(std::shared_ptr<AudioComponent> jump, std::shared_ptr<AudioComponent> fall, std::shared_ptr<AudioComponent> swear);
	
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

	std::shared_ptr<AudioComponent> m_JumpSound;
	std::shared_ptr<AudioComponent> m_FallSound;
	std::shared_ptr<AudioComponent> m_SwearSound;
	
	int m_PlayerNbr;

	bool m_bCanMove;
	bool m_bWillSleep;
};
