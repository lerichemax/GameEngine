#pragma once
#include "Character.h"


struct AudioComponent;
struct CharacterLives;
class CharacterPoint;
struct RendererComponent;
class QBert final : public Character
{
public:
	QBert();
	~QBert() = default;

	int GetPlayerNumber() const { return m_PlayerNbr; }
	
	void EarnPoints(int points);
	void JumpOffDisk();
	void Reset(bool fullReset, QubeSystem* pTargetQube);
	void SetCanMove() { m_bCanMove = true; }
	void SetPlayerNbr(int nbr) { m_PlayerNbr = nbr; }
	int GetPlayerNbr() const { return m_PlayerNbr; }
	void Swear()const;
	void SetAudioComponents(AudioComponent* const jump, AudioComponent* const fall, AudioComponent* const swear);

	void Serialize(StreamWriter& writer) const ;
	void Deserialize(JsonReader const* reader, SerializationMap& context) ;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) ;

protected:
	void Initialize() ;
	void Start() ;
	void Update() ;

	void MeetCharacter(Character* pOther) override;
	void LandOnQube() override;
	void DoDie() override;
	void DoMove(ConnectionDirection direction) override;

private:
	CharacterPoint* m_pPoints;
	CharacterLives* m_pLives;
	RendererComponent* m_pHurtTex;

	AudioComponent* m_pJumpSound;
	AudioComponent* m_pFallSound;
	AudioComponent* m_pSwearSound;
	
	int m_PlayerNbr;

	bool m_bCanMove;
	bool m_bWillSleep;
};
