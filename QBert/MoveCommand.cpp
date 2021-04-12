#include "PCH.h"
#include "MoveCommand.h"

#include "QBert.h"

MoveCommand::MoveCommand(ConnectionDirection dir, QBert* pQbert, Pyramid* pController, KeyActionState state)
	: Command(state),
	m_MoveDirection{ dir },
	m_pQbert(pQbert),
	m_pGameController(pController)
{}

bool MoveCommand::Execute()
{
	m_pQbert->Move(m_MoveDirection);
	return true;
}