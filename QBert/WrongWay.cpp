#include "PCH.h"
#include "WrongWay.h"

#include "GameObject.h"
#include "Qube.h"
#include "ResourceManager.h"
#include "Timer.h"

WrongWay::WrongWay(bool escheresqueRight)
	:Enemy(0),
	m_bEscheresqueRight(escheresqueRight)
{
	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay.png");
}

void WrongWay::Initialize()
{
	m_pJumpText = ResourceManager::GetInstance().GetTexture("Textures/Enemies/WrongWay/WrongWay.png");
	Debugger::GetInstance().AddDebugPoint(glm::vec2{ 100,100 }, 2, Color(255, 0, 0));
}

void WrongWay::Move(ConnectionDirection direction)
{
	if (m_pCurrentQube->HasEscheresqueConnection(direction, m_bEscheresqueRight))
	{
		auto pNextQube = m_pCurrentQube->GetEscheresqueConnection(direction, m_bEscheresqueRight);

		m_pCurrentQube->CharacterJumpOut();
		JumpToQube(pNextQube);
	}
	else
	{
		JumpToDeath(direction);
	}
}

void WrongWay::Update()
{
	auto rot = m_pGameObject->GetTransform()->GetRotation();
	m_pGameObject->GetTransform()->Rotate(rot + 45 * Timer::GetInstance().GetDeltaTime());

	
}