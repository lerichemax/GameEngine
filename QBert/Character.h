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
	Character(Qube* pStart, CharacterType type);
	Character(Character&& other) = delete;
	Character& operator=(Character const& rhs) = delete;
	Character& operator=(Character&& rhs) = delete;

	virtual Character* Clone() const override = 0;
	
	virtual ~Character();
	
	virtual void Update(); 
	virtual void Move(ConnectionDirection direction) = 0;
	virtual void Die() = 0;
	
	Qube* const GetCurrentQube() const { return m_pCurrentQube; }
	CharacterType GetType() const { return m_Type; }
	CharacterState* GetState() const { return m_pState; }

	void SetCurrentQube(Qube* pTargetQube);
	virtual void JumpToQube(Qube* pTargetQube);
	void JumpToDeath(ConnectionDirection dir);

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
	
	virtual void Initialize() override;
	virtual void MeetCharacter(Character* pOther) = 0; // Find itself on the same cube as another character
	virtual void SetDirectionTextures(ConnectionDirection dir) = 0;
	void SwitchToIdleTex();
	void SwitchToJumpTex();
	virtual void MoveToCurrentQube();
	virtual void LandOnQube() {};
	virtual void SetLayerToBackGround();
	
	void SwitchState(CharacterState* pState);
	void SwitchState(CharacterStateType type);
	
	
	Character(Character const& other);
	
};
