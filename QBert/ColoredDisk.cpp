#include "PCH.h"
#include "ColoredDisk.h"
#include "Qube.h"

#include "GameObject.h"
#include "QBert.h"
#include "Timer.h"

ColoredDisk::ColoredDisk(Qube* top)
	:PYRAMID_TOP(top)
{
}

void ColoredDisk::Update()
{
	if (m_bHasQbert)
	{
		if (!m_bHasReachedTop)
		{
			auto finalPos = PYRAMID_TOP->GetGameObject()->GetTransform()->GetPosition();
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
				m_pQbert->SetCurrentQube(PYRAMID_TOP);
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