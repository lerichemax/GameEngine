#include "PCH.h"
#include "CharacterPoint.h"

CharacterPoint::CharacterPoint()
	:Component(true),
	m_NbrPoints()
{

}

void CharacterPoint::AddPoints(int nbrPoint)
{ 
	m_NbrPoints += nbrPoint; 
	OnPointsUpdated.Notify(m_NbrPoints, GetEntity()); 
}
