#include "PCH.h"
#include "WrongWay.h"
#include "Qube.h"

WrongWay::WrongWay(bool escheresqueRight)
	:Enemy(0),
	m_bEscheresqueRight(escheresqueRight)
{
	
}

void WrongWay::Move(ConnectionDirection direction)
{
	if (m_pCurrentQube->HasEscheresqueConnection(direction, m_bEscheresqueRight))
	{
		auto pNextQube = m_pCurrentQube->GetEscheresqueConnection(direction, m_bEscheresqueRight);

		m_pCurrentQube->CharacterJumpOut();
		JumpToQube(pNextQube);
	}
	else
	{
		JumpToDeath(direction);
	}
}