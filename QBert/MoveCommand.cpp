#include "PCH.h"
#include "MoveCommand.h"

#include "CharacterState.h"
#include "GameObject.h"
#include "CharacterMovement.h"

MoveCommand::MoveCommand(ConnectionDirection dir, std::shared_ptr<CharacterMovement> pMover)
	: m_MoveDirection{ dir },
	m_pMover(pMover)
{}

MoveCommand::MoveCommand(MoveCommand const& other)
	:m_pMover(other.m_pMover),
	m_MoveDirection(other.m_MoveDirection)
{
}

void MoveCommand::Execute()
{
	if (!m_pMover->GetGameObject()->IsActive()/* || m_pCharacter->GetState()->GetType() != CharacterStateType::onQube*/)
	{
		return;
	}
	
	m_pMover->Move(m_MoveDirection);
}