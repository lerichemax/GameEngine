#include "PCH.h"
#include "WrongWay.h"
#include "Jumper.h"

#include "GameObject.h"
#include "Qube.h"
#include "RendererComponent.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "BoxCollider.h"

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
	
	Character::Initialize();
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
		m_bEscheresqueRight ? m_pCurrentQube->GetEscheresqueRight() : m_pCurrentQube->GetEscheresqueLeft());
}

void WrongWay::Die()
{
	m_pGameObject->Destroy();
	m_pSubject->Notify(this, (int)EnemyEvents::WrongWayDies);
}

void WrongWay::MoveToCurrentQube()
{
	m_pGameObject->GetTransform()->Translate(m_pCurrentQube->GetCharacterPos());
}

void WrongWay::Update()
{
	
	if (m_State == State::onQube)
	{
		if (m_MoveTimer < MOVE_MAX_TIME)
		{
			m_MoveTimer += empire::Timer::GetInstance().GetDeltaTime();
			return;
		}

		Move(ChooseDirection());
		m_MoveTimer = 0;
	}

	if (m_State == State::jumping)
	{
		m_pJumper->UpdateJump(m_pGameObject->GetTransform());
		if (!m_pJumper->IsJumping())
		{
			m_State = State::onQube;
			//LandOnQube();
			MoveToCurrentQube();
		}
	}
	else if (m_State == State::falling)
	{
		m_pJumper->UpdateFall(m_pGameObject->GetTransform());
		if (m_pJumper->IsDead())
		{
			m_pJumper->SetIsNotDead();
			m_State = State::onQube;
			Die();
		}
	}
}

ConnectionDirection WrongWay::ChooseDirection()
{
	int random = rand() % 2 + 1; //down left or down right

	auto dir = (ConnectionDirection)random;

	if (!m_pCurrentQube->HasEscheresqueConnection(dir, m_bEscheresqueRight))
	{
		if (dir == ConnectionDirection::downLeft)
		{
			dir = ConnectionDirection::downRight;
		}
		else if (dir == ConnectionDirection::downRight)
		{
			dir = ConnectionDirection::downLeft;
		}
	}

	return dir;
}