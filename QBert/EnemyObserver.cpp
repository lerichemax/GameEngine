#include "PCH.h"
#include "EnemyObserver.h"
#include "Pyramid.h"
#include "Coily.h"
#include "SlickSam.h"
#include "WrongWay.h"
#include "EnemyManager.h"

EnemyObserver::EnemyObserver(EnemyManager* const pManager)
	:Observer(),
	m_pEnemyManager(pManager)
{
}

void EnemyObserver::Notify(empire::Component* object, int event)
{
	switch ((EnemyEvents)event)
	{
	case EnemyEvents::CoilyDies:
		m_pEnemyManager->CoilyDied(static_cast<Coily*>(object));
		break;
	case EnemyEvents::SlickSamDies:
		m_pEnemyManager->SlickSamDied(static_cast<SlickSam*>(object));
		break;
	case EnemyEvents::WrongWayDies:
		m_pEnemyManager->WrongWayDied(static_cast<WrongWay*>(object));
		break;
	}
}