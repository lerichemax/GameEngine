#include "PCH.h"
#include "QBert.h"
#include "PlayerObserver.h"
#include "Qube.h"
#include "ColoredDisk.h"
#include "Coily.h"
#include "SlickSam.h"

#include "GameObject.h"
#include "Jumper.h"
#include "Subject.h"
#include "ResourceManager.h"
#include "RendererComponent.h"

using namespace empire;

int QBert::m_PlayerNbr = 0;

QBert::QBert()
	:Character(nullptr, Type::player),
	m_NbrLives{ 3 },
	m_NbrPoints{},
	m_bCanMove(true)
{
	m_PlayerNbr++;
}

void QBert::Initialize()
{
	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_DownLeft_Qube.png");
	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_DownLeft_Jump.png");
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIdleText);
	Character::Initialize();
}

QBert::~QBert()
{
	m_PlayerNbr--;
}

void QBert::Die()
{
	m_NbrLives--;
	m_bCanMove = false;
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
	if (!m_bCanMove)
	{
		return;
	}
	
	if (!m_pCurrentQube->HasConnection(direction) && !m_pCurrentQube->HasConnectionToDisk())
	{
		JumpToDeath(direction);
		return;
	}

	SetDirectionTextures(direction);
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pJumpText);
	
	if (m_pCurrentQube->HasConnection(direction))
	{
		m_pCurrentQube->CharacterJumpOut();
		JumpToQube(m_pCurrentQube->GetConnection(direction));
	}
	else if(m_pCurrentQube->HasConnectionToDisk())
	{
		m_pCurrentQube->GetConnectedDisk()->ReceivePlayer(this);
		m_pCurrentQube->CharacterJumpOut();
		m_pCurrentQube = nullptr;
		m_bCanMove = false;
	}
}

void QBert::JumpOffDisk()
{
	m_State = State::onQube;
	m_pSubject->Notify(this, (int)PlayerEvent::JumpOffDisk);
	m_bCanMove = true;
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

void QBert::MeetCharacter(Character* pOther)
{
	if (typeid(*pOther) == typeid(Coily) && static_cast<Coily*>(pOther)->IsTransformed())
	{
		Die();
	}
	else if (typeid(*pOther) == typeid(SlickSam))
	{
		auto pSlickSam = static_cast<SlickSam*>(pOther);
		EarnPoints(pSlickSam->GetPointsForKill());
		pSlickSam->Die();
	}
}

void QBert::LandOnQube()
{
	m_pCurrentQube->QBertJump();
	Character::LandOnQube();
}

void QBert::SetDirectionTextures(ConnectionDirection dir)
{
	if (m_FacingDirection == dir)
	{
		return;
	}

	m_FacingDirection = dir;

	switch (dir)
	{
	case ConnectionDirection::downLeft:
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_DownLeft_Qube.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_DownLeft_Jump.png");
		break;
	case ConnectionDirection::downRight:
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_DownRight_Qube.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_DownRight_Jump.png");
		break;
	case ConnectionDirection::upLeft:
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_UpLeft_Qube.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_UpLeft_Jump.png");
		break;
	case ConnectionDirection::upRight:
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_UpRight_Qube.png");
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert_UpRight_Jump.png");
		break;
	}
}