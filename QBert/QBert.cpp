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
#include "Pyramid.h"
#include "Jumper.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "RendererComponent.h"
#include "BoxCollider.h"
#include "GameManager.h"
#include "OnQubeState.h"
#include "VersusGameManager.h"
#include "CharacterMovement.h"
#include "AudioComponent.h"

QBert::QBert()
	:Character(nullptr, CharacterType::player),
	m_pPoints(nullptr),
	m_pLives(nullptr),
	m_pHurtTex{ nullptr },
	m_pJumpSound(nullptr),
	m_pFallSound(nullptr),
	m_pSwearSound(nullptr),
	m_PlayerNbr(),
	m_bCanMove(true),
	m_bWillSleep(false)	
{
}

void QBert::Initialize()
{
	m_pPoints = GetGameObject()->GetComponent<ECS_CharacterPoint>();
	m_pLives = GetGameObject()->GetComponent<ECS_CharacterLives>();
	
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
	m_pHurtTex = GetGameObject()->GetComponentInChildren<RendererComponent>();
	m_pHurtTex->SetActive(false);

	Character::Initialize();
}

void QBert::Start()
{
	auto pyramid = FindComponentOfType<Pyramid>();
	auto pMover = GetGameObject()->GetComponent<CharacterMovement>();
	pMover->SetCurrentQube(pyramid->GetTop());

	pMover->OnMoveStarted.Subscribe([this]() {
		m_pJumpSound->Play();
		});

	pyramid->OnAllQubesFlipped.Subscribe([this](int points) {
		EarnPoints(points);
		});

	Qube::OnAnyQubeFlipped.Subscribe([this]() {
		EarnPoints(Qube::POINTS_FOR_FLIP);
		});

	auto jumper = GetGameObject()->GetComponent<Jumper>();
	jumper->OnJumpedToDeath.Subscribe([this]() {
		m_pFallSound->Play();
		GetGameObject()->GetComponent<RendererComponent>()->m_Layer = 1;
		});

	jumper->OnFell.Subscribe([this]() {
		m_pLives->Die();
		Reset(false, GetGameObject()->GetComponent<CharacterMovement>()->GetCurrentQube());
		});

}

void QBert::Update()
{
	if (m_bWillSleep)
	{
		if (m_pHurtTex != nullptr)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			m_pHurtTex->SetActive(false);
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
		m_pHurtTex->SetActive(true);
		m_bWillSleep = true;

	}

	//if (m_pLives->IsGameOver())
	//{
	//	GetGameObject()->Notify(static_cast<int>(GameEvent::GameOver));
	//}
	//else
	//{
	//	GetGameObject()->Notify(static_cast<int>(GameEvent::PlayerDied));
	//	GetGameObject()->Notify(static_cast<int>(VersusGameEvent::Player1Died));
	//}
}

void QBert::Swear()const
{
	m_pSwearSound->Play();
}

void QBert::SetAudioComponents(std::shared_ptr<AudioComponent> jump, std::shared_ptr<AudioComponent> fall, std::shared_ptr<AudioComponent> swear)
{
	m_pJumpSound = jump;
	m_pFallSound = fall;
	m_pSwearSound = swear;
}

void QBert::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(QBert).name());
	writer.WriteInt("jumpId", m_pJumpSound->GetId());
	writer.WriteInt("fallId", m_pFallSound->GetId());
	writer.WriteInt("swearId", m_pSwearSound->GetId());

	Character::Serialize(writer);
}

void QBert::Deserialize(JsonReader const* reader, SerializationMap& context)
{
}

void QBert::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	int id = -1;
	reader->ReadInt("jumpId", id);
	m_pJumpSound = context.GetRef<AudioComponent>(id);
	reader->ReadInt("fallId", id);
	m_pFallSound = context.GetRef<AudioComponent>(id);
	reader->ReadInt("swearId", id);
	m_pSwearSound = context.GetRef<AudioComponent>(id);
}

void QBert::EarnPoints(int points)
{
	m_pPoints->AddPoints(points);
}

void QBert::DoMove(ConnectionDirection direction)
{
	printf("move \n");
	if (!m_bCanMove)
	{
		return;
	}
	
	//if (!m_pCurrentQube->HasConnection(direction) && !m_pCurrentQube->HasConnectionToDisk())
	//{
		JumpToDeath(direction);
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
	//GetGameObject()->Notify(static_cast<int>(GameEvent::JumpOffDisk));
	//m_pGameObject->GetComponent<BoxCollider>()->SetEnable(true);
	m_bCanMove = true;
}

void QBert::Reset(bool fullReset, std::shared_ptr<Qube> pTargetQube)
{
	GetGameObject()->GetComponent<CharacterMovement>()->SetCurrentQube(pTargetQube);
	GetGameObject()->SetActive(true, false);
	GetGameObject()->GetComponent<RendererComponent>()->m_Layer = 8;

	if (!fullReset)
	{
		return;
	}
	SwitchState(new OnQubeState(this, m_pJumper));
	
	m_pLives->Reset();
	m_pPoints->Reset();
	
	//Notify these events to update the HUD
	//GetGameObject()->Notify(static_cast<int>(GameEvent::PlayerDied));
	//GetGameObject()->Notify(static_cast<int>(GameEvent::IncreasePoints));
}

void QBert::MeetCharacter(Character* pOther) 
{
	if (pOther->GetType() == CharacterType::coily && static_cast<Coily*>(pOther)->IsTransformed() || 
		pOther->GetType() == CharacterType::wrongWay)
	{
		m_pSwearSound->Play();
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