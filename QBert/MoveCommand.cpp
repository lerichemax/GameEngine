#include "PCH.h"
#include "MoveCommand.h"



#include "CharacterState.h"
#include "GameObject.h"
#include "QBert.h"

MoveCommand::MoveCommand(ConnectionDirection dir, Character* pChar)
	: m_MoveDirection{ dir },
	m_pCharacter(pChar)
{}

MoveCommand::MoveCommand(MoveCommand const& other)
	:m_pCharacter(other.m_pCharacter),
	m_MoveDirection(other.m_MoveDirection)
{
}

void MoveCommand::Execute()
{
	if (!m_pCharacter->GetGameObject()->IsActive() || m_pCharacter->GetState()->GetType() != CharacterStateType::onQube)
	{
		return;
	}
	
	m_pCharacter->Move(m_MoveDirection);
}