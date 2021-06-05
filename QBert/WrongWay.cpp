#include "PCH.h"
#include "WrongWay.h"

#include "BoxCollider.h"
#include "EnemyOnQubeState.h"

#include "Jumper.h"
#include "GameManager.h"
#include "JumpingState.h"
#include "Qube.h"
#include "EnemyCharacterController.h"

#include "GameObject.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

WrongWay::WrongWay(bool escheresqueRight)
	:Enemy(0, CharacterType::wrongWay),
	m_bEscheresqueRight(escheresqueRight)
{

}

void WrongWay::Initialize()
{
	if (m_bEscheresqueRight)
	{
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay_Left.png");
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay_Left.png");
	}
	else
	{
		m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/Ugg_Left.png");
		m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/Ugg_Left.png");
	}

	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIdleText);
	m_pJumper = m_pGameObject->GetComponent<Jumper>();

	if (m_bEscheresqueRight)
	{
		m_pGameObject->GetTransform()->SetWorldPosition(m_pCurrentQube->GetEscheresqueRightPos());
	}
	else
	{
		m_pGameObject->GetTransform()->SetWorldPosition(m_pCurrentQube->GetEscheresqueLeftPos());
	}
	SwitchState(new EnemyOnQubeState(this, m_pJumper, m_pGameObject->GetComponent<EnemyCharacterController>()));
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
	m_pJumper->Jump(m_pGameObject->GetTransform()->GetWorldPosition(),
		m_bEscheresqueRight ? m_pCurrentQube->GetEscheresqueRightPos() : m_pCurrentQube->GetEscheresqueLeftPos());
}

void WrongWay::Die()
{
	m_pGameObject->Destroy();
	m_pGameObject->Notify((int)GameEvent::WrongWayDies);
}

void WrongWay::SetDirectionTextures(ConnectionDirection dir)
{
	if (m_FacingDirection == dir)
	{
		return;
	}

	m_FacingDirection = dir;

	switch (dir)
	{
	case ConnectionDirection::downLeft:
		if (m_bEscheresqueRight)
		{
			m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay_Left.png");
			m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/WrongWay_Left.png");
		}
		else
		{
			m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/Ugg_Left.png");
			m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/Ugg_Left.png");
		}

		break;
	case ConnectionDirection::downRight:
		if (m_bEscheresqueRight)
		{
			m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay_Right.png");
			m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/WrongWay_Right.png");
		}
		else
		{
			m_pIdleText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/Ugg_Right.png");
			m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/SlickSam/Ugg_Right.png");
		}
		break;
	}
}