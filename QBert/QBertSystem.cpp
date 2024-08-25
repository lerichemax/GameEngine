#include "PCH.h"

#include "QBertSystem.h"
#include "PyramidSystem.h"

#include "CharacterControllerComponent.h"
#include "CharacterComponent.h"
#include "QubeComponent.h"

#include "Coordinator.h"

void QBertSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<CharacterControllerComponent>());
	signature.set(pRegistry->GetComponentType<CharacterComponent>());

	pRegistry->SetSystemSignature<QBertSystem>(signature);
}


void QBertSystem::Start()
{
	for (Entity entity : m_Entities)
	{
		auto pCharacter = m_pCompManager->GetComponent<CharacterComponent>(entity);
		pCharacter->CurrentQube = GetSystem<PyramidSystem>()->GetTop();

		auto pQube = m_pCompManager->GetComponent<QubeComponent>(pCharacter->CurrentQube);

		auto pTransform = m_pCompManager->GetComponent<ECS_TransformComponent>(entity);
		pTransform->Translate(pQube->CharacterPos);
	}
}