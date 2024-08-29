#pragma once
#include "Component.h"

enum class CharacterType //enum to avoid using typeid
{
	player,
	slickSam,
	coily,
	wrongWay
};

class Texture2D;

class Jumper;
class QubeSystem;
class CharacterState;
enum class ConnectionDirection;
enum class CharacterStateType;
class Character 
{
public:
	explicit Character(QubeSystem* const pStart, CharacterType type);
	explicit Character() = default;
	Character(Character&& other) = delete;
	Character& operator=(Character const& rhs) = delete;
	Character& operator=(Character&& rhs) = delete;
	
	virtual ~Character();
	
	void Update(); 
	
	QubeSystem* const GetCurrentQube() const { return m_pCurrentQube; }
	CharacterType GetType() const { return m_Type; }
	CharacterState* GetState() const { return m_pState; }

	void SetCurrentQube(QubeSystem* const pTargetQube);
	void JumpToDeath(ConnectionDirection dir);
	void Move(ConnectionDirection direction);
	void Die();

protected:
	friend class OnQubeState;
	friend class JumpingState;
	friend class FallingState;
	
	QubeSystem* m_pCurrentQube;
	Jumper* m_pJumper;
	
	CharacterState* m_pState;
	
	Texture2D* m_pJumpText;
	Texture2D* m_pIdleText;
	
	CharacterType m_Type;
	ConnectionDirection m_FacingDirection;
	
	void Start();
	virtual void DoMove(ConnectionDirection direction) = 0;
	virtual void DoDie() = 0;
	virtual void MeetCharacter(Character* pOther) = 0; // Find itself on the same cube as another character
	virtual void SetDirectionTextures(ConnectionDirection dir) = 0;
	virtual void JumpToQube(QubeSystem* const pTargetQube);
	virtual void MoveToCurrentQube();
	virtual void LandOnQube() {};
	virtual void SetLayerToBackGround();
	
	void SwitchToIdleTex() const;
	void SwitchToJumpTex() const;
	void SwitchState(CharacterState* pState);
	void SwitchState(CharacterStateType type);
	
	Character(Character const& other);
};
