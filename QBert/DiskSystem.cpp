#include "PCH.h"
#include "DiskSystem.h"
#include "LivesSystem.h"

#include "DiskComponent.h"
#include "QubeComponent.h"
#include "RendererComponent.h"
#include "TransformComponent.h"
#include "QbertComponent.h"

#include "Timer.h"
#include "Texture2D.h"

void DiskSystem::Start()
{
	m_pRegistry->GetSystem<LivesSystem>()->OnDied.Subscribe([this](Entity deadEntity, int nbrLives) {
		if (m_pRegistry->EntityHasTag(deadEntity, QBERT_TAG))
		{
			auto view = m_pRegistry->GetView<DiskComponent, TransformComponent>();
			for (Entity entity : view)
			{
				m_pRegistry->SetEntityActive(entity, false);
			}
		}
	});
}

void DiskSystem::Update()
{
	auto view = m_pRegistry->GetView<DiskComponent, TransformComponent>();

	for (Entity entity : view)
	{
		auto* const pDiskComp = m_pRegistry->GetComponent<DiskComponent>(entity);
		if (!pDiskComp->IsActive() || pDiskComp->QbertEntity == NULL_ENTITY)
		{
			continue;
		}

		if (!pDiskComp->bHasReachedTop)
		{
			auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

			glm::vec2 distance{ pDiskComp->TargetPosition - pTransform->GetLocation() };
			glm::vec2 direction{ glm::normalize(distance) };

			direction *= (pDiskComp->MOVE_SPEED * Timer::Get().GetDeltaTime());
			pTransform->SetLocation(pTransform->GetLocation() + direction);

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
				pDiskComp->DropTimer += Timer::Get().GetDeltaTime();
			}
			else
			{
				OnDiskReachedTop.Notify(entity);
				m_pRegistry->SetEntityActive(entity, false);
				pDiskComp->QbertEntity = NULL_ENTITY;
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

	auto view = m_pRegistry->GetView<DiskComponent, TransformComponent>();

	Entity diskEntity = *std::find_if(view.begin(), view.end(), [this](Entity entity) {
		auto* const pDisk = m_pRegistry->GetComponent<DiskComponent>(entity);
		return !pDisk->IsActive();
		});

	m_pRegistry->GetComponent<DiskComponent>(diskEntity)->LinkedQube = qubeEntity;

	m_pRegistry->SetEntityHierarchyActive(diskEntity, true);

	auto const parentPos = pQubeTransform->GetLocation();

	auto* const pDiskTransform = m_pRegistry->GetComponent<TransformComponent>(diskEntity);
	if (!pQube->HasConnection(ConnectionDirection::upLeft))
	{
		pDiskTransform->SetLocation(parentPos.x - pQubeRenderer->pTexture->GetWidth() / 1.5f, parentPos.y - pQubeRenderer->pTexture->GetHeight() / 1.75f);
		pQube->ConnectionToDisk->Direction = ConnectionDirection::upLeft;
	}
	else
	{
		pDiskTransform->SetLocation(parentPos.x + pQubeRenderer->pTexture->GetWidth() * 1.5f, parentPos.y - pQubeRenderer->pTexture->GetHeight() / 1.75f);
		pQube->ConnectionToDisk->Direction = ConnectionDirection::upRight;
	}

	pQube->ConnectionToDisk->Disk = diskEntity;
	LOG_INFO("Disk spawned");
}
