#include "PCH.h"
#include "WrongWay.h"
#include "Jumper.h"

#include "GameObject.h"
#include "Qube.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

#include "GameManager.h"



WrongWay::WrongWay(bool escheresqueRight)
	:Enemy(0),
	m_bEscheresqueRight(escheresqueRight)
{

}



void WrongWay::Initialize()
{
	if (m_bEscheresqueRight)
	{
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay.png");
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay.png");
	}
	else
	{
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWayLeft.png");
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWayLeft.png");
	}

	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIdleText);
	m_pJumper = m_pGameObject->GetComponent<Jumper>();

	if (m_bEscheresqueRight)
	{
		m_pGameObject->GetTransform()->Translate(m_pCurrentQube->GetEscheresqueRightPos());
	}
	else
	{
		m_pGameObject->GetTransform()->Translate(m_pCurrentQube->GetEscheresqueLeftPos());
	}
	
}

void WrongWay::Move(ConnectionDirection direction)
{
	if (m_pCurrentQube->HasEscheresqueConnection(direction, m_bEscheresqueRight))
	{
		auto pNextQube = m_pCurrentQube->GetEscheresqueConnection(direction, m_bEscheresqueRight);

		JumpToQube(pNextQube);
	}
	else
	{
		JumpToDeath(direction);
	}
}

void WrongWay::JumpToQube(Qube* pNextQube)
{
	if (pNextQube == nullptr)
	{
		return;
	}

	m_pCurrentQube = pNextQube;
	m_State = State::jumping;
	m_pJumper->Jump(m_pGameObject->GetTransform()->GetPosition(), 
		m_bEscheresqueRight ? m_pCurrentQube->GetEscheresqueRightPos() : m_pCurrentQube->GetEscheresqueLeftPos());
}

void WrongWay::Die()
{
	m_pGameObject->Destroy();
	m_pGameObject->Notify((int)GameEvent::WrongWayDies);
}