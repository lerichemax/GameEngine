#include "PCH.h"
#include "Character.h"

#include "CharacterState.h"
#include "CoilyCharacterController.h"
#include "Jumper.h"
#include "Qube.h"
#include "OnQubeState.h"
#include "JumpingState.h"
#include "FallingState.h"
#include "CoilyOnQubeState.h"
#include "EnemyOnQubeState.h"

#include "GameObject.h"
#include "RendererComponent.h"

Character::Character(Qube* pStart, CharacterType type)
	: m_pCurrentQube(pStart),
	m_pJumper(nullptr),
	m_pState(nullptr),
	m_pJumpText(nullptr),
	m_pIdleText(nullptr),
	m_Type(type),
	m_FacingDirection(ConnectionDirection::downLeft)
	
{
}

Character::Character(Character const& other)
	:m_pCurrentQube(other.m_pCurrentQube),
	m_Type(other.m_Type),
	m_pJumper(nullptr),
	m_pState(nullptr),
	m_pIdleText(other.m_pIdleText),
	m_pJumpText(other.m_pJumpText),
	m_FacingDirection(other.m_FacingDirection)
{
}


Character::~Character()
{
	SafeDelete(m_pState);
}

void Character::Initialize()
{
	m_pJumper = m_pGameObject->GetComponent<Jumper>();
	SwitchState(CharacterStateType::onQube);
	MoveToCurrentQube();
}

void Character::Update()
{
	auto newState = m_pState->Update(m_pGameObject);
	SwitchState(newState);
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
	m_pJumper->Jump(m_pGameObject->GetTransform()->GetWorldPosition(), m_pCurrentQube->GetCharacterPos());
}

void Character::SwitchToIdleTex() const
{
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIdleText);
}

void Character::SwitchToJumpTex() const
{
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pJumpText);
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
	
	SetLayerToBackGround();
	
	m_pCurrentQube->CharacterJumpOut();
	m_pJumper->JumpToDeath(m_pGameObject->GetTransform()->GetWorldPosition(), dist);
}

void Character::Move(ConnectionDirection direction)
{
	DoMove(direction);
}

void Character::Die()
{
	DoDie();
}

void Character::SetLayerToBackGround()
{
	if (!m_pCurrentQube->IsLastRow())
	{
		m_pGameObject->GetComponent<RendererComponent>()->ChangeLayer(Layer::preBacground);
	}
}

void Character::SwitchState(CharacterState* pState)
{
	if (pState != nullptr)
	{
		if (m_pState != nullptr)
		{
			m_pState->Exit();
			SafeDelete(m_pState);
		}
		m_pState = pState;
		m_pState->Enter();
	}
}

void Character::SwitchState(CharacterStateType type)
{
	if (m_pState != nullptr && m_pState->GetType() == type)
	{
		return;
	}
	switch (type)
	{
	case CharacterStateType::onQube:
		switch (m_Type)
		{
		case CharacterType::player:
			SwitchState(new OnQubeState(this, m_pJumper));
			break;
		case CharacterType::coily:
			if (m_pGameObject->HasComponent<CoilyCharacterController>())
			{

				SwitchState(new CoilyOnQubeState(this, m_pJumper, 
					m_pGameObject->GetComponent<CoilyCharacterController>()));
			}
			else
			{
				SwitchState(new OnQubeState(this, m_pJumper));
			}
			break;
		default:
			SwitchState(new EnemyOnQubeState(this, m_pJumper,
				m_pGameObject->GetComponent<EnemyCharacterController>()));
			break;
		}
		
		break;
	case CharacterStateType::jumping:
		SwitchState(new JumpingState(this, m_pJumper));
		break;
	case CharacterStateType::falling:
		SwitchState(new FallingState(this, m_pJumper));
		break;
	}
}