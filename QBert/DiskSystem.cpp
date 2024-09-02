#include "PCH.h"
#include "DiskSystem.h"

#include "DiskComponent.h"
#include "QubeComponent.h"
#include "RendererComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"

#include "Texture2D.h"

void DiskSystem::Update()
{
	for (Entity entity : m_Entities)
	{
		auto* const pDiskComp = m_pRegistry->GetComponent<DiskComponent>(entity);
		if (!pDiskComp->IsActive() || !pDiskComp->bHasQbert)
		{
			continue;
		}

		if (!pDiskComp->bHasReachedTop)
		{
			auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

			glm::vec2 distance{ pDiskComp->TargetPosition - pTransform->GetLocation() };
			glm::vec2 direction{ glm::normalize(distance) };

			direction *= (pDiskComp->MOVE_SPEED * Timer::GetInstance().GetDeltaTime());
			pTransform->Translate(pTransform->GetLocation() + direction);

			distance = pDiskComp->TargetPosition - pTransform->GetLocation();
			if (glm::length(distance) <= 1.f)
			{
				pDiskComp->bHasReachedTop = true;
			}
		}
		else
		{
			if (pDiskComp->DropTimer < pDiskComp->DROP_MAX_TIME)
			{
				pDiskComp->DropTimer += Timer::GetInstance().GetDeltaTime();
			}
			else
			{
				OnDiskReachedTop.Notify(entity);
				m_pRegistry->SetEntityActive(entity, false);
				pDiskComp->bHasQbert = false;
				pDiskComp->bHasReachedTop = false;
				pDiskComp->DropTimer = 0;
			}
		}
	}
}

void DiskSystem::SpawnDisk(Entity qubeEntity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
	auto* const pQubeTransform = m_pRegistry->GetComponent<TransformComponent>(qubeEntity);
	auto* const pQubeRenderer = m_pRegistry->GetComponent<RendererComponent>(qubeEntity);

	Entity diskEntity = *std::find_if(m_Entities.begin(), m_Entities.end(), [this](Entity entity) {
		auto* const pDisk = m_pRegistry->GetComponent<DiskComponent>(entity);
		return !pDisk->IsActive();
		});

	m_pRegistry->SetEntityHierarchyActive(diskEntity, true);

	//pDisk->GetComponent<DiskSystem>()->SetPyramidTop(m_pGameObject->GetParent()->GetComponent<Pyramid>()->GetTop());

	auto const parentPos = pQubeTransform->GetLocation();

	auto* const pDiskTransform = m_pRegistry->GetComponent<TransformComponent>(diskEntity);
	if (!pQube->HasConnection(ConnectionDirection::upLeft))
	{
		pDiskTransform->Translate(parentPos.x - pQubeRenderer->pTexture->GetWidth() / 1.5f, parentPos.y - pQubeRenderer->pTexture->GetHeight() / 1.75f);
	}
	else
	{
		pDiskTransform->Translate(parentPos.x + pQubeRenderer->pTexture->GetWidth() * 1.5f, parentPos.y - pQubeRenderer->pTexture->GetHeight() / 1.75f);
	}

	pQube->ConnectionToDisk = diskEntity;
	Debugger::GetInstance().Log("Disk spawned");
}

void DiskSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<DiskComponent>());

	m_pRegistry->SetSystemSignature<DiskSystem>(signature);
}

void DiskSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(DiskSystem)).hash_code()));
}