#include "PCH.h"
#include "LivesSystem.h"

#include "CharacterLives.h"
#include "QbertComponent.h"

void LivesSystem::Update()
{
	for (Entity entity : m_Entities)
	{
		auto* const pLivesComp = m_pRegistry->GetComponent<CharacterLives>(entity);

		if (pLivesComp->bIsDead)
		{
			OnDied.Notify(entity, pLivesComp->NbrLives);
			pLivesComp->bIsDead = false;

			if (pLivesComp->NbrLives <= 0 && m_pRegistry->HasTag(entity, QBERT_TAG))
			{
				OnPlayerDied.Notify();
				m_pRegistry->SetEntityHierarchyActive(entity, false);
			}
		}
	}
}

void LivesSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(LivesSystem)).hash_code()));
}

void LivesSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<CharacterLives>());
	m_pRegistry->SetSystemSignature<LivesSystem>(signature);
}