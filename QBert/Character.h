#pragma once
#include "Component.h"
#include "Subject.h"

enum class CharacterType //enum to avoid using typeid
{
	player,
	slickSam,
	coily,
	wrongWay
};

namespace empire
{
	class Texture2D;
}

class Jumper;
class Qube;
class CharacterState;
enum class ConnectionDirection;
enum class CharacterStateType;
class Character : public empire::Component
{
public:
	explicit Character(Qube* pStart, CharacterType type);
	Character(Character&& other) = delete;
	Character& operator=(Character const& rhs) = delete;
	Character& operator=(Character&& rhs) = delete;

	virtual Character* Clone() const override = 0;
	
	virtual ~Character();
	
	void Update() override; 
	
	Qube* const GetCurrentQube() const { return m_pCurrentQube; }
	CharacterType GetType() const { return m_Type; }
	CharacterState* GetState() const { return m_pState; }

	void SetCurrentQube(Qube* pTargetQube);
	void JumpToDeath(ConnectionDirection dir);
	void Move(ConnectionDirection direction);
	void Die();

protected:
	friend class OnQubeState;
	friend class JumpingState;
	friend class FallingState;
	
	Qube* m_pCurrentQube;
	Jumper* m_pJumper;
	
	CharacterState* m_pState;
	
	empire::Texture2D* m_pJumpText;
	empire::Texture2D* m_pIdleText;
	
	CharacterType m_Type;
	ConnectionDirection m_FacingDirection;
	
	void Initialize() override;
	virtual void DoMove(ConnectionDirection direction) = 0;
	virtual void DoDie() = 0;
	virtual void MeetCharacter(Character* pOther) = 0; // Find itself on the same cube as another character
	virtual void SetDirectionTextures(ConnectionDirection dir) = 0;
	virtual void JumpToQube(Qube* pTargetQube);
	virtual void MoveToCurrentQube();
	virtual void LandOnQube() {};
	virtual void SetLayerToBackGround();
	
	void SwitchToIdleTex();
	void SwitchToJumpTex();
	void SwitchState(CharacterState* pState);
	void SwitchState(CharacterStateType type);
	
	Character(Character const& other);
};
