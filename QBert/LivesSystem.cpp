#include "PCH.h"
#include "LivesSystem.h"

#include "CharacterLives.h"
#include "QbertComponent.h"
#include "CharacterControllerComponent.h"

void LivesSystem::Update()
{
	auto view = m_pRegistry->GetView<CharacterLives>();

	for (Entity entity : view)
	{
		auto* const pLivesComp = m_pRegistry->GetComponent<CharacterLives>(entity);

		if (pLivesComp->bIsDead)
		{
			OnDied.Notify(entity, pLivesComp->NbrLives);
			pLivesComp->bIsDead = false;

			if (pLivesComp->NbrLives <= 0)
			{
				auto* const pController = m_pRegistry->GetComponent<CharacterControllerComponent>(entity);
				if (IS_VALID(pController))
				{
					OnPlayerDied.Notify(entity);
					m_pRegistry->SetEntityHierarchyActive(entity, false);
				}
			}
		}
	}
}
