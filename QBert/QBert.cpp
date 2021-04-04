#include "PCH.h"
#include "QBert.h"

#include "GameObject.h"
#include "Qube.h"
#include "TransformComponent.h"

#include "Subject.h"
#include "PlayerObserver.h"
using namespace empire;

int QBert::m_PlayerNbr = 0;

QBert::QBert(Qube* pCurrentCube)
	:m_NbrLives{ 3 },
	m_NbrPoints{},
	m_pSubject{new Subject{}},
	m_pCurrentQube(pCurrentCube)
{
	m_PlayerNbr++;
}

void QBert::Init(empire::GameObject* pParent)
{
	Component::Init(pParent);
	m_pParentObject->GetTransform()->Translate(m_pCurrentQube->GetQbertPos());
}

QBert::QBert()
	:m_NbrLives{ 3 },
	m_NbrPoints{},
	m_pSubject{ new Subject{} },
	m_pCurrentQube()
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
	m_pSubject->Notify(this, (int)PlayerEvent::PlayerDied);

}

void QBert::EarnPoints()
{
	m_NbrPoints++;
	m_pSubject->Notify(this, (int)PlayerEvent::IncreasePoints);
}

void QBert::Move(Qube* pTargetQube)
{
	m_pCurrentQube = pTargetQube;
	m_pParentObject->GetTransform()->Translate(pTargetQube->GetQbertPos());
	m_pCurrentQube->Flip();
}