#include "PCH.h"
#include "EnemyObserver.h"
#include "Pyramid.h"
#include "Coily.h"
#include "SlickSam.h"
#include "WrongWay.h"
#include "EnemyManager.h"

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
		m_pPyramid->GetEnemyManager()->CoilyDied(static_cast<Coily*>(object));
		break;
	case EnemyEvents::SlickSamDies:
		m_pPyramid->GetEnemyManager()->SlickSamDied(static_cast<SlickSam*>(object));
		break;
	case EnemyEvents::WrongWayDies:
		m_pPyramid->GetEnemyManager()->WrongWayDied(static_cast<WrongWay*>(object));
		break;
	}
}