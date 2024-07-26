#pragma once
#include "Character.h"


struct AudioComponent;
struct ECS_CharacterLives;
class ECS_CharacterPoint;
struct ECS_RendererComponent;
class QBert final : public Character
{
public:
	QBert();
	~QBert() = default;

	int GetPlayerNumber() const { return m_PlayerNbr; }
	
	void EarnPoints(int points);
	void JumpOffDisk();
	void Reset(bool fullReset, std::shared_ptr<Qube> pTargetQube);
	void SetCanMove() { m_bCanMove = true; }
	void SetPlayerNbr(int nbr) { m_PlayerNbr = nbr; }
	int GetPlayerNbr() const { return m_PlayerNbr; }
	void Swear()const;
	void SetAudioComponents(std::shared_ptr<AudioComponent> jump, std::shared_ptr<AudioComponent> fall, std::shared_ptr<AudioComponent> swear);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

protected:
	void Initialize() override;
	void Start() override;
	void Update() override;

	void MeetCharacter(Character* pOther) override;
	void LandOnQube() override;
	void DoDie() override;
	void DoMove(ConnectionDirection direction) override;
	void SetDirectionTextures(ConnectionDirection dir) override;

private:
	std::shared_ptr<ECS_CharacterPoint> m_pPoints;
	std::shared_ptr<ECS_CharacterLives> m_pLives;
	std::shared_ptr<ECS_RendererComponent> m_pHurtTex;

	std::shared_ptr<AudioComponent> m_pJumpSound;
	std::shared_ptr<AudioComponent> m_pFallSound;
	std::shared_ptr<AudioComponent> m_pSwearSound;
	
	int m_PlayerNbr;

	bool m_bCanMove;
	bool m_bWillSleep;
};
