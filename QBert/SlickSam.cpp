#include "PCH.h"
#include "SlickSam.h"

#include "GameManagerSystem.h"

#include "GameObject.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

int SlickSam::m_NbrInstances{};

SlickSam::SlickSam()
	:Enemy(300, CharacterType::slickSam),
	m_bIsSam(false)
{
	m_NbrInstances++;
}

SlickSam::SlickSam(SlickSam const& other)
	:Enemy(other),
	m_bIsSam(false)
{
	m_NbrInstances++;
}

void SlickSam::Initialize()
{
	//Enemy::Initialize();
	SetTexture();
}

SlickSam::~SlickSam()
{
	m_NbrInstances--;
}

void SlickSam::SetTexture()
{
	//if (m_NbrInstances == 1)
	//{
	//	m_bIsSam = false;
	//	m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Slick_Down_Left.png");
	//	m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Slick_Up_Left.png");
	//}
	//else if (m_NbrInstances == 2)
	//{
	//	m_bIsSam = true;
	//	m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Sam_Down_Left.png");
	//	m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Sam_Up_Left.png");
	//}
	
	//m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIdleText);
}

void SlickSam::DoDie()
{
	Enemy::DoDie();
	//GetEntity()->Notify(static_cast<int>(GameEvent::SlickSamDies));
}

void SlickSam::MeetCharacter(Characters* pOther)
{
	if (pOther->GetType() == CharacterType::player)
	{
		//dynamic_cast<QBert*>(pOther)->EarnPoints(POINTS_FOR_KILL);
		DoDie();
	}
}

void SlickSam::SetDirectionTextures(ConnectionDirection dir)
{
	if (m_FacingDirection == dir)
	{
		return;
	}

	m_FacingDirection = dir;

	//switch (dir)
	//{
	//case ConnectionDirection::downLeft:
	//	if (m_bIsSam)
	//	{
	//		m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Sam_Down_Left.png");
	//		m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Sam_Up_Left.png");
	//	}
	//	else
	//	{
	//		m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Slick_Down_Left.png");
	//		m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Slick_Up_Left.png");
	//	}
	//	
	//	break;
	//case ConnectionDirection::downRight:
	//	if (m_bIsSam)
	//	{
	//		m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Sam_Down_Right.png");
	//		m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Sam_Up_Right.png");
	//	}
	//	else
	//	{
	//		m_pIdleText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Slick_Down_Right.png");
	//		m_pJumpText = ResourceManager::Get().GetTexture("Textures/Enemies/SlickSam/Slick_Up_Right.png");
	//	}
	//	break;
	//}
}