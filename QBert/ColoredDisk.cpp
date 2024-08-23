#include "PCH.h"
#include "ColoredDisk.h"
#include "Qube.h"
#include "QBert.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"

ColoredDisk::ColoredDisk()
	:m_pPyramidTop(nullptr),
	m_pQbert(nullptr),
	m_bHasQbert(false),
	m_bHasReachedTop(false),
	m_DropTimer(0.f)
{
}

ColoredDisk::ColoredDisk(ColoredDisk const& other)
	:m_pPyramidTop(other.m_pPyramidTop),
	m_pQbert(other.m_pQbert),
	m_bHasQbert(other.m_bHasQbert),
	m_bHasReachedTop(other.m_bHasReachedTop),
	m_DropTimer(other.m_DropTimer)
{
	
}

void ColoredDisk::Update()
{
	/*if (m_bHasQbert)
	{
		if (!m_bHasReachedTop)
		{
			if (m_pPyramidTop == nullptr)
			{
				Debugger::GetInstance().LogError("Variable m_pPyramid top of ColoredDisk is undefined");
			}
			glm::vec2 finalPos = m_pPyramidTop->GetGameObject()->GetECSTransform()->GetWorldPosition();
			finalPos.y += OFFSET;
			
			TransformComponent* parentTrans{ m_pGameObject->GetECSTransform() };
			TransformComponent* qBertTrans{ m_pQbert->GetGameObject()->GetECSTransform() };
			glm::vec2 distance{ finalPos - parentTrans->GetWorldPosition() };
			glm::vec2 direction{ glm::normalize(distance) };

			direction *= (MOVE_SPEED * Timer::GetInstance().GetDeltaTime());
			parentTrans->SetWorldPosition(parentTrans->GetWorldPosition() + direction);
			qBertTrans->Translate(qBertTrans->GetWorldPosition() + direction);
			
			distance = finalPos - parentTrans->GetWorldPosition();
			if (glm::length(distance) <= 1)
			{
				m_bHasReachedTop = true;
			}
		}
		else
		{
			if (m_DropTimer < DROP_MAX_TIME)
			{
				m_DropTimer += Timer::GetInstance().GetDeltaTime();
			}
			else
			{
				m_pQbert->SetCurrentQube(m_pPyramidTop);
				m_pQbert->JumpOffDisk();
				m_pQbert = nullptr;
				m_pGameObject->Destroy();
			}
		}*/
	//}
}

void ColoredDisk::ReceivePlayer(QBert* pQbert)
{
	m_pQbert = pQbert;
	//m_pQbert->GetGameObject()->GetTransform()->Translate(m_pGameObject->GetTransform()->GetPosition());
	m_bHasQbert = true;
}