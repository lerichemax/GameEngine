#pragma once
#include "Component.h"
#include "Subject.h"

enum class Type //enum to avoid using typeid
{
	player,
	enemy
};

enum class State
{
	onQube,
	jumping,
	falling
};

namespace empire
{
	class Texture2D;
}

class Jumper;
class Qube;
enum class ConnectionDirection;
class Character : public empire::Component
{
public:
	Character(Qube* pStart, Type type);
	Character(Character&& other) = delete;
	Character& operator=(Character const& rhs) = delete;
	Character& operator=(Character&& rhs) = delete;

	virtual Character* Clone() override = 0;
	
	virtual ~Character() = default;
	
	virtual void Update(); 
	virtual void Move(ConnectionDirection direction) = 0;
	virtual void Die() = 0;
	
	Qube* const GetCurrentQube() const { return m_pCurrentQube; }
	Type GetType() const { return m_Type; }
	State GetState() const { return m_State; }

	void SetCurrentQube(Qube* pTargetQube);
	virtual void JumpToQube(Qube* pTargetQube);//temp
	void Jump() { m_State = State::jumping; }
	void JumpToDeath(ConnectionDirection dir);

protected:
	Qube* m_pCurrentQube;
	Jumper* m_pJumper;
	
	empire::Texture2D* m_pJumpText;
	empire::Texture2D* m_pIdleText;
	
	Type m_Type;
	State m_State;
	ConnectionDirection m_FacingDirection;
	
	virtual void Initialize() override;
	virtual void MeetCharacter(Character* pOther) = 0; // Find itself on the same cube as another character
	virtual void SetDirectionTextures(ConnectionDirection dir) = 0;
	
	virtual void LandOnQube();
	virtual void MoveToCurrentQube();

	Character(Character const& other);
	
};
