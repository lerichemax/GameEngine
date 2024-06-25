#include "PCH.h"
#include "QBert.h"

#include <chrono>
#include <thread>

#include "Qube.h"
#include "ColoredDisk.h"
#include "Coily.h"
#include "SlickSam.h"
#include "WrongWay.h"
#include "CharacterPoint.h"
#include "CharacterLives.h"
#include "JumpingState.h"
#include "FallingState.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "RendererComponent.h"
#include "BoxCollider.h"
#include "GameManager.h"
#include "OnQubeState.h"
#include "SoundServiceLocator.h"
#include "VersusGameManager.h"

QBert::QBert(std::shared_ptr<AudioComponent> jump, std::shared_ptr<AudioComponent> fall, std::shared_ptr<AudioComponent> swear)
	:Character(nullptr, CharacterType::player),
	m_pPoints(nullptr),
	m_pLives(nullptr),
	m_pHurtTex{ nullptr },
	m_JumpSound(jump),
	m_FallSound(fall),
	m_SwearSound(swear),
	m_PlayerNbr(),
	m_bCanMove(true),
	m_bWillSleep(false)	
{
}

void QBert::Initialize()
{
	//m_pPoints = m_pGameObject->GetComponent<CharacterPoint>();
	//m_pLives = m_pGameObject->GetComponent<CharacterLives>();
	
	//m_pGameObject->GetComponent<BoxCollider>()->SetIsTrigger(true);
	//m_pGameObject->GetComponent<BoxCollider>()->SetOnTriggerEnter([this](GameObject*, GameObject* pOther)
	//	{
	//		if (pOther->HasComponent<WrongWay>())
	//		{
	//			//MeetCharacter(pOther->GetComponent<WrongWay>());
	//		}
	//	});
	
	//m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert"+std::to_string(m_PlayerNbr)+"_DownLeft_Qube.png");
	//m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_DownLeft_Jump.png");
	m_pHurtTex = m_pGameObject->GetComponentInChildren<RendererComponent>();
	m_pHurtTex->SetEnable(false);
	Character::Initialize();
}

void QBert::Update()
{
	if (m_bWillSleep)
	{
		if (m_pHurtTex != nullptr)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			m_pHurtTex->SetEnable(false);
			m_bWillSleep = false;
		}
	}
	Character::Update();
}
void QBert::DoDie()
{
	m_pLives->Die();
	m_bCanMove = false;
	
	if (m_pHurtTex != nullptr)
	{
		m_pHurtTex->SetEnable(true);
		m_bWillSleep = true;

	}

	if (m_pLives->IsGameOver())
	{
		m_pGameObject->Notify(static_cast<int>(GameEvent::GameOver));
	}
	else
	{
		m_pGameObject->Notify(static_cast<int>(GameEvent::PlayerDied));
		m_pGameObject->Notify(static_cast<int>(VersusGameEvent::Player1Died));
	}
}

void QBert::Swear()const
{
	//SoundServiceLocator::GetService().Play(m_SwearSoundID, 50);
}

void QBert::EarnPoints(int points)
{
	m_pPoints->AddPoints(points);
	m_pGameObject->Notify(static_cast<int>(GameEvent::IncreasePoints));
	m_pGameObject->Notify(static_cast<int>(VersusGameEvent::IncreasePoints));
}

void QBert::DoMove(ConnectionDirection direction)
{
	if (!m_bCanMove)
	{
		return;
	}
	
	//if (!m_pCurrentQube->HasConnection(direction) && !m_pCurrentQube->HasConnectionToDisk())
	//{
	//	JumpToDeath(direction);
	//	SwitchState(new FallingState(this, m_pJumper));
	//	SoundServiceLocator::GetService().Play(m_FallSoundID, 50);
	//	return;
	//}

	//SetDirectionTextures(direction);
	//
	//if (m_pCurrentQube->HasConnection(direction))
	//{
	//	SoundServiceLocator::GetService().Play(m_JumpSoundID, 50);
	//	m_pCurrentQube->CharacterJumpOut();
	//	JumpToQube(m_pCurrentQube->GetConnection(direction));
	//	SwitchState(new JumpingState(this, m_pJumper));
	//}
	//else if(m_pCurrentQube->HasConnectionToDisk())
	//{
	//	SoundServiceLocator::GetService().Play(m_JumpSoundID, 50);
	//	//m_pGameObject->GetComponent<BoxCollider>()->SetEnable(false);
	//	m_pCurrentQube->GetConnectedDisk()->ReceivePlayer(this);
	//	m_pCurrentQube->CharacterJumpOut();
	//	m_pCurrentQube = nullptr;
	//	m_bCanMove = false;
	//}
}

void QBert::JumpOffDisk()
{
	SwitchState(new OnQubeState(this, m_pJumper));
	m_pGameObject->Notify(static_cast<int>(GameEvent::JumpOffDisk));
	//m_pGameObject->GetComponent<BoxCollider>()->SetEnable(true);
	m_bCanMove = true;
}

void QBert::Reset(bool fullReset, Qube* pTargetQube)
{
	SetCurrentQube(pTargetQube);
	m_pGameObject->SetActive(true);
	if (!fullReset)
	{
		return;
	}
	SwitchState(new OnQubeState(this, m_pJumper));
	
	m_pLives->Reset();
	m_pPoints->Reset();
	
	//Notify these events to update the HUD
	m_pGameObject->Notify(static_cast<int>(GameEvent::PlayerDied));
	m_pGameObject->Notify(static_cast<int>(GameEvent::IncreasePoints));
}

void QBert::MeetCharacter(Character* pOther) 
{
	if (pOther->GetType() == CharacterType::coily && static_cast<Coily*>(pOther)->IsTransformed() || 
		pOther->GetType() == CharacterType::wrongWay)
	{
		Swear();
		DoDie();
	}
	else if (pOther->GetType() == CharacterType::slickSam)
	{
		auto pSlickSam = dynamic_cast<SlickSam*>(pOther);
		EarnPoints(pSlickSam->GetPointsForKill());
		pSlickSam->Die();
	}
}

void QBert::LandOnQube()
{
	m_pCurrentQube->QBertJump(this);
}

void QBert::SetDirectionTextures(ConnectionDirection dir)
{
	if (m_FacingDirection == dir)
	{
		return;
	}

	m_FacingDirection = dir;

	//switch (dir)
	//{
	//case ConnectionDirection::downLeft:
	//	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_DownLeft_Qube.png");
	//	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_DownLeft_Jump.png");
	//	break;
	//case ConnectionDirection::downRight:
	//	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_DownRight_Qube.png");
	//	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_DownRight_Jump.png");
	//	break;
	//case ConnectionDirection::upLeft:
	//	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_UpLeft_Qube.png");
	//	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_UpLeft_Jump.png");
	//	break;
	//case ConnectionDirection::upRight:
	//	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_UpRight_Qube.png");
	//	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/QBert/QBert" + std::to_string(m_PlayerNbr) + "_UpRight_Jump.png");
	//	break;
	//}
}