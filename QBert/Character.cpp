#include "PCH.h"
#include "Character.h"
#include "Jumper.h"
#include "Qube.h"

#include "GameObject.h"
#include "RendererComponent.h"

Character::Character(Qube* pStart, Type type)
	: m_pCurrentQube(pStart),
	m_pSubject(new empire::Subject{}),
	m_Type(type),
	m_State(State::onQube),
	m_pJumper(new Jumper{}),
	m_pIdleText(nullptr),
	m_pJumpText(nullptr),
	m_FacingDirection(ConnectionDirection::downLeft)
{
}

void Character::Initialize()
{
	MoveToCurrentQube();
}

Character::~Character()
{
	delete m_pJumper;
}

void Character::Update()
{
	if (m_State == State::jumping)
	{
		m_pJumper->Update(m_pGameObject->GetTransform());
		if (!m_pJumper->IsJumping())
		{
			m_State = State::onQube;
			MoveToCurrentQube();
			LandOnQube();
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
	
	m_pCurrentQube->CharacterJumpIn(this);
	//m_State = State::jumping;
	m_pGameObject->GetTransform()->Translate(m_pCurrentQube->GetCharacterPos());
}

void Character::JumpToQube(Qube* pTargetQube)
{
	if (pTargetQube == nullptr)
	{
		return;
	}

	m_pCurrentQube = pTargetQube;
	m_State = State::jumping;
	m_pJumper->Jump(m_pGameObject->GetTransform()->GetPosition(), m_pCurrentQube->GetCharacterPos());
}

void Character::LandOnQube()
{
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIdleText);
}