#include "PCH.h"
#include "Character.h"
#include "Jumper.h"
#include "Qube.h"

#include "GameObject.h"
#include "RendererComponent.h"

Character::Character(Qube* pStart, Type type)
	: m_pCurrentQube(pStart),
	m_Type(type),
	m_State(State::onQube),
	m_pJumper(),
	m_pIdleText(nullptr),
	m_pJumpText(nullptr),
	m_FacingDirection(ConnectionDirection::downLeft)
{
}

Character::Character(Character const& other)
	:m_pCurrentQube(other.m_pCurrentQube),
	m_Type(other.m_Type),
	m_State(other.m_State),
	m_pJumper(),
	m_pIdleText(other.m_pIdleText),
	m_pJumpText(other.m_pJumpText),
	m_FacingDirection(other.m_FacingDirection)
{

}

void Character::Initialize()
{
	m_pJumper = m_pGameObject->GetComponent<Jumper>();
	m_State = State::onQube;
	MoveToCurrentQube();
}

void Character::Update()
{
	if (m_State == State::jumping)
	{
		m_pJumper->UpdateJump(m_pGameObject->GetTransform());
		if (!m_pJumper->IsJumping())
		{
			m_State = State::onQube;
			MoveToCurrentQube();
			LandOnQube();
		}
	}
	else if(m_State == State::falling)
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

void Character::SetCurrentQube(Qube* pTargetQube)
{
	if (pTargetQube == nullptr)
	{
		return;
	}

	m_pCurrentQube = pTargetQube;
	MoveToCurrentQube();
}

void Character::MoveToCurrentQube()
{
	if (m_pCurrentQube == nullptr)
	{
		return;
	}

	if (m_pCurrentQube->HasCharacter() && m_pCurrentQube->GetCharacter() != this)
	{
		MeetCharacter(m_pCurrentQube->GetCharacter());
	}

	if (m_pGameObject->IsActive())
	{
		m_State = State::onQube;
		m_pCurrentQube->CharacterJumpIn(this);
		m_pGameObject->GetTransform()->SetWorldPosition(m_pCurrentQube->GetCharacterPos());
	}
}

void Character::JumpToQube(Qube* pTargetQube)
{
	if (pTargetQube == nullptr)
	{
		return;
	}

	m_pCurrentQube = pTargetQube;
	m_State = State::jumping;
	m_pJumper->Jump(m_pGameObject->GetTransform()->GetWorldPosition(), m_pCurrentQube->GetCharacterPos());
}

void Character::LandOnQube()
{
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIdleText);
}

void Character::JumpToDeath(ConnectionDirection dir)
{
	float dist{};
	if (dir == ConnectionDirection::downLeft || dir == ConnectionDirection::upLeft)
	{
		dist = -25.f;
	}
	else if(dir == ConnectionDirection::downRight || dir == ConnectionDirection::upRight)
	{
		dist = 25.f;
	}
	
	if (!m_pCurrentQube->IsLastRow())
	{
		m_pGameObject->GetComponent<RendererComponent>()->ChangeLayer(Layer::preBacground);
	}
	
	m_pCurrentQube->CharacterJumpOut();
	m_pJumper->JumpToDeath(m_pGameObject->GetTransform()->GetWorldPosition(), dist);
	m_State = State::falling;
}