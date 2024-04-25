#pragma once
#include "State.h"



class GameObject;
class OnQubeState;
class JumpingState;
class FallingState;
class Jumper;
class Character;

enum class CharacterStateType // enum to avoid using typeid
{
	onQube,
	jumping,
	falling
};

class CharacterState : public State
{
public:

	explicit CharacterState(Character* pChar, Jumper* pJumper, CharacterStateType type);
	virtual ~CharacterState() = default;
	
	
	virtual void Enter() override{};
	virtual void Exit() override{};

	CharacterStateType GetType()const { return m_Type; }
	CharacterState* Update(GameObject*) override { return nullptr; }

protected:
	Jumper* m_pJumperComp;
	Character* m_pCharacter;

private:
	CharacterStateType m_Type;
	
	
	void HandleInput(GameObject* ) override {};
};