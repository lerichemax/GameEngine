#include "PCH.h"
#include "SlickSam.h"
#include "QBert.h"

#include "GameObject.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

int SlickSam::m_NbrInstances{};

SlickSam::SlickSam(Qube* pQube)
	:Enemy(pQube, 300)
{
	m_NbrInstances++;
}

void SlickSam::Initialize()
{
	Enemy::Initialize();
	SetTexture();
}

SlickSam::~SlickSam()
{
	m_NbrInstances--;
}

void SlickSam::SetTexture()
{
	if (m_NbrInstances == 1)
	{
		m_pGameObject->GetComponent<empire::RendererComponent>()->SetTexture(
			empire::ResourceManager::GetInstance().GetTexture("Slick.png"));
	}
	else if (m_NbrInstances == 2)
	{
		m_pGameObject->GetComponent<empire::RendererComponent>()->SetTexture(
			empire::ResourceManager::GetInstance().GetTexture("Sam.png"));
	}
}

void SlickSam::Die()
{
	Enemy::Die();
	m_pSubject->Notify(this, (int)EnemyEvents::SlickSamDies);
}

void SlickSam::MeetCharacter(Character* pOther)
{
	if (pOther->GetType() == Type::player)
	{
		static_cast<QBert*>(pOther)->EarnPoints(POINTS_FOR_KILL);
		Die();
	}
}