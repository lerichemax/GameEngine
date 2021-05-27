#include "PCH.h"
#include "ColoredDisk.h"
#include "Qube.h"

#include "GameObject.h"
#include "QBert.h"
#include "Timer.h"

ColoredDisk::ColoredDisk()
	:m_pPyramidTop(nullptr),
	m_pQbert(nullptr),
	m_bHasQbert(false),
	m_bHasReachedTop(false),
	m_DropTimer(0.f)
{
}

void ColoredDisk::Update()
{
	if (m_bHasQbert)
	{
		if (!m_bHasReachedTop)
		{
			if (m_pPyramidTop == nullptr)
			{
				Debugger::GetInstance().LogError("Variable m_pPyramid top of ColoredDisk is undefined");
			}
			auto finalPos = m_pPyramidTop->GetGameObject()->GetTransform()->GetPosition();
			finalPos.y += OFFSET;
			
			auto parentTrans = m_pGameObject->GetTransform();
			auto qBertTrans = m_pQbert->GetGameObject()->GetTransform();
			auto distance = finalPos - parentTrans->GetPosition();
			auto direction = glm::normalize(distance);

			direction *= (MOVE_SPEED * Timer::GetInstance().GetDeltaTime());
			parentTrans->Translate(parentTrans->GetPosition() + direction);
			qBertTrans->Translate(qBertTrans->GetPosition() + direction);
			
			distance = finalPos - parentTrans->GetPosition();
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
		}
	}
}

void ColoredDisk::ReceivePlayer(QBert* pQbert)
{
	m_pQbert = pQbert;
	m_pQbert->GetGameObject()->GetTransform()->Translate(m_pGameObject->GetTransform()->GetPosition());
	m_bHasQbert = true;
}