#include "MiniginPCH.h"
#include "QBert.h"
#include "TransformComponent.h"
#include "PlayerObserver.h"
#include "Subject.h"
using namespace dae;

int QBert::m_PlayerNbr = 0;

QBert::QBert()
	:m_NbrLives{ 3 },
	m_NbrPoints{},
	m_pSubject{new Subject{}}
{
	m_PlayerNbr++;
}

QBert::~QBert()
{
	m_PlayerNbr--;
	delete m_pSubject;
}

void QBert::Die()
{
	m_NbrLives--;
	m_pSubject->Notify(this, Event::PlayerDied);

}

void QBert::EarnPoints()
{
	m_NbrPoints++;
	m_pSubject->Notify(this, Event::IncreasePoints);
}