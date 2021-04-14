#include "PCH.h"
#include "EnemyObserver.h"

#include "Pyramid.h"
#include "Coily.h"

EnemyObserver::EnemyObserver(Pyramid* const pPyramid)
	:Observer(),
	m_pPyramid(pPyramid)
{
	
}

void EnemyObserver::Notify(empire::Component* object, int event)
{
	switch ((EnemyEvents)event)
	{
	case EnemyEvents::CoilyDies:
		m_pPyramid->CoilyDied(static_cast<Coily*>(object));
		break;
	case EnemyEvents::SlickSamDies:
		m_pPyramid->SlickSamDied();
		break;
	}
}