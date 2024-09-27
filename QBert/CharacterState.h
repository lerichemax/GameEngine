#pragma once

class GameObject;
class OnQubeState;
class JumpingState;
class FallingState;
class Jumper;
class Characters;

enum class CharacterStateType // enum to avoid using typeid
{
	onQube,
	jumping,
	falling
};

class CharacterState 
{
public:

	explicit CharacterState(Characters* pChar, Jumper* pJumper, CharacterStateType type);
	virtual ~CharacterState() = default;
	
	
	virtual void Enter() {};
	virtual void Exit() {};

	CharacterStateType GetType()const { return m_Type; }
	CharacterState* Update(GameObject*)  { return nullptr; }

protected:
	Jumper* m_pJumperComp;
	Characters* m_pCharacter;

private:
	CharacterStateType m_Type;
	
	
	void HandleInput(GameObject* ) {};
};