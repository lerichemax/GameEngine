#include "PCH.h"
#include "MoveCommand.h"

#include "QBert.h"

MoveCommand::MoveCommand(ConnectionDirection dir, QBert* pQbert, Pyramid* pController, KeyActionState state)
	: Command(state),
	m_MoveDirection{ dir },
	m_pQbert(pQbert),
	m_pGameController(pController)
{}

void MoveCommand::Execute()
{
	if (m_pQbert->GetState() != State::onQube)
	{
		return;
	}
	
	m_pQbert->Move(m_MoveDirection);
}