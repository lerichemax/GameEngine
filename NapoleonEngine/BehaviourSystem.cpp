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
	for (Entity const& entity : m_ToBeAdded)
	{
		auto behaviour = pComponentManager->GetComponent<BehaviourComponent>(entity);
		if (behaviour->IsActive())
		{
			behaviour->Initialize();
		}

		m_Entities.insert(entity);
	}

	m_ToBeAdded.clear();

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

void BehaviourSystem::AddEntity(Entity entity)
{
	m_ToBeAdded.push_back(entity);
}