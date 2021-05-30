#include "PCH.h"
#include "MoveCommand.h"


#include "GameObject.h"
#include "QBert.h"

MoveCommand::MoveCommand(ConnectionDirection dir, Character* pChar)
	: m_MoveDirection{ dir },
	m_pCharacter(pChar)
{}

void MoveCommand::Execute()
{
	if (!m_pCharacter->GetGameObject()->IsActive() || m_pCharacter->GetState() != State::onQube)
	{
		return;
	}
	
	m_pCharacter->Move(m_MoveDirection);
}