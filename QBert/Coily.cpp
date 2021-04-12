#include "PCH.h"
#include "Coily.h"

#include "GameObject.h"
#include "RendererComponent.h"
#include "ResourceManager.h"

Coily::Coily(Pyramid* pPyramid)
	: Enemy(),
	m_pPyramid(pPyramid),
	m_bIsTransformed(false)
{}

void Coily::Update()
{
	if (!m_bIsTransformed)
	{
		Enemy::Update();
		if (m_pCurrentQube->IsLastRow())
		{
			Transform();
		}
	}
	else if (!m_bIsIdle)
	{
		Enemy::Update();
	}
}

void Coily::Transform()
{
	m_pGameObject->GetComponent<RendererComponent>()->
		SetTexture(empire::ResourceManager::GetInstance().GetTexture("Coily_FaceRight_Tall.png"));
	m_bIsTransformed = true;
}

ConnectionDirection Coily::ChooseDirection() const
{
	if (!m_bIsTransformed)
	{
		return Enemy::ChooseDirection();
	}

	return m_pPyramid->FindNextQubeToQbert(m_pCurrentQube);
}