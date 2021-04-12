#include "PCH.h"
#include "QBert.h"

#include "GameObject.h"
#include "Subject.h"

#include "PlayerObserver.h"
#include "Qube.h"
#include "ColoredDisk.h"

using namespace empire;

int QBert::m_PlayerNbr = 0;
//
//QBert::QBert(Qube* pCurrentQube)
//	:Character(pCurrentQube),
//	m_NbrLives{ MAX_LIVES },
//	m_NbrPoints{}
//{
//	m_PlayerNbr++;
//}

QBert::QBert()
	:Character(),
	m_NbrLives{ 3 },
	m_NbrPoints{}
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
	if (m_NbrLives <= 0)
	{
		m_pSubject->Notify(this, (int)PlayerEvent::GameOver);
	}
	else
	{
		m_pSubject->Notify(this, (int)PlayerEvent::PlayerDied);
	}
}

void QBert::EarnPoints(int points)
{
	m_NbrPoints += points;
	m_pSubject->Notify(this, (int)PlayerEvent::IncreasePoints);
}

void QBert::Move(ConnectionDirection direction)
{
	if (!m_pCurrentQube->HasConnection(direction) && !m_pCurrentQube->HasConnectionToDisk())
	{
		Die();
		return;
	}

	if (m_pCurrentQube->HasConnection(direction))
	{
		m_pCurrentQube->CharacterJumpOut();
		SetCurrentQube(m_pCurrentQube->GetConnection(direction));
		m_pCurrentQube->QBertJump();
	}
	else if(m_pCurrentQube->HasConnectionToDisk())
	{
		m_pCurrentQube->GetConnectedDisk()->ReceivePlayer(this);
		m_pSubject->Notify(this, (int)PlayerEvent::JumpOnDisk);
		m_pCurrentQube = nullptr;
	}
}

void QBert::JumpOffDisk()
{
	m_pSubject->Notify(this, (int)PlayerEvent::JumpOffDisk);
}

void QBert::Reset(bool fullReset, Qube* pTargetQube)
{
	SetCurrentQube(pTargetQube);

	if (!fullReset)
	{
		return;
	}

	m_NbrLives = MAX_LIVES;
	m_NbrPoints = 0;

	//Notify these events to update the HUD
	m_pSubject->Notify(this, (int)PlayerEvent::PlayerDied);
	m_pSubject->Notify(this, (int)PlayerEvent::IncreasePoints);
}