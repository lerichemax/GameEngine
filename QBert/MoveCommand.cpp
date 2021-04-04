#include "PCH.h"
#include "MoveCommand.h"

#include "QBert.h"

MoveCommand::MoveCommand(ConnectionDirection dir, QBert* pQbert, GameController* pController, KeyActionState state)
	: Command(state),
	m_MoveDirection{ dir },
	m_pQbert(pQbert),
	m_pGameController(pController)
{}

bool MoveCommand::Execute()
{
	if (!m_pQbert->GetCurrentQube()->HasConnection(m_MoveDirection))
	{
		std::cout << "This direction is not available for your current position\n";
		return true;
	}

	auto target = m_pQbert->GetCurrentQube()->GetConnection(m_MoveDirection);
	m_pQbert->Move(target);
	return true;
}