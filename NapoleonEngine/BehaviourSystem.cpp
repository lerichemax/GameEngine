#include "PCH.h"
#include "BehaviourSystem.h"
#include "BehaviourComponent.h"

void BehaviourSystem::Initialize(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto behaviour = pComponentManager->GetComponent<BehaviourComponent>(entity);
		behaviour->Initialize();
	}
}

void BehaviourSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto behaviour = pComponentManager->GetComponent<BehaviourComponent>(entity);
		if (!behaviour->IsActive())
		{
			continue;
		}

		behaviour->Update();
	}
}

void BehaviourSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<BehaviourComponent>(), true);

	pRegistry->SetSystemSignature<BehaviourSystem>(signature);
}