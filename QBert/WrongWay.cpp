#include "PCH.h"
#include "WrongWay.h"

#include "BoxCollider.h"
#include "EnemyOnQubeState.h"

#include "GameManager.h"
#include "JumpingState.h"
#include "EnemyCharacterController.h"

#include "GameObject.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

WrongWay::WrongWay(bool escheresqueRight)
	:Enemy(0, CharacterType::wrongWay),
	m_bEscheresqueRight(escheresqueRight)
{

}

WrongWay::WrongWay(WrongWay const& other)
	:Enemy(other),
	m_bEscheresqueRight(other.m_bEscheresqueRight)
{
	
}

void WrongWay::Initialize()
{
	//if (m_bEscheresqueRight)
	//{
	//	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay_Left.png");
	//	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay_Left.png");
	//}
	//else
	//{
	//	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/Ugg_Left.png");
	//	m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/Ugg_Left.png");
	//}

	//m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIdleText);
	//m_pJumper = m_pGameObject->GetComponent<Jumper>();

	if (m_bEscheresqueRight)
	{
		//m_pGameObject->GetECSTransform()->SetWorldPosition(m_pCurrentQube->GetEscheresqueRightPos());
		//m_pGameObject->GetTransform()->Translate(m_pCurrentQube->GetEscheresqueRightPos());
	}
	else
	{
		//m_pGameObject->GetECSTransform()->SetWorldPosition(m_pCurrentQube->GetEscheresqueLeftPos());
		//m_pGameObject->GetTransform()->Translate(m_pCurrentQube->GetEscheresqueLeftPos());
	}
	//SwitchState(new EnemyOnQubeState(this, m_pJumper, m_pGameObject->GetComponent<EnemyCharacterController>()));
}

void WrongWay::DoMove(ConnectionDirection direction)
{
	//if (m_pCurrentQube->HasEscheresqueConnection(direction, m_bEscheresqueRight))
	//{
	//	auto pNextQube = m_pCurrentQube->GetEscheresqueConnection(direction, m_bEscheresqueRight);

	//	JumpToQube(pNextQube);
	//}
	//else
	//{
	//	JumpToDeath(direction);
	//}
}

void WrongWay::JumpToQube(QubeSystem* const pNextQube)
{
	if (pNextQube == nullptr)
	{
		return;
	}

	m_pCurrentQube = pNextQube;
	//m_pJumper->Jump(m_pGameObject->GetTransform()->GetWorldPosition(),
	//	m_bEscheresqueRight ? m_pCurrentQube->GetEscheresqueRightPos() : m_pCurrentQube->GetEscheresqueLeftPos());
}

void WrongWay::DoDie()
{
	GetGameObject()->Destroy();
	//GetGameObject()->Notify(static_cast<int>(GameEvent::WrongWayDies));
}

void WrongWay::SetDirectionTextures(ConnectionDirection dir)
{
	if (m_FacingDirection == dir)
	{
		return;
	}

	m_FacingDirection = dir;

	//switch (dir)
	//{
	//case ConnectionDirection::downLeft:
	//	if (m_bEscheresqueRight)
	//	{
	//		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay_Left.png");
	//		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/WrongWay_Left.png");
	//	}
	//	else
	//	{
	//		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/Ugg_Left.png");
	//		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/Ugg_Left.png");
	//	}

	//	break;
	//case ConnectionDirection::downRight:
	//	if (m_bEscheresqueRight)
	//	{
	//		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay_Right.png");
	//		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/WrongWay_Right.png");
	//	}
	//	else
	//	{
	//		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/Ugg_Right.png");
	//		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/Ugg_Right.png");
	//	}
	//	break;
	//}
}