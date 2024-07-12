#include "PCH.h"
#include "BehaviourSystem.h"
#include "BehaviourComponent.h"

void BehaviourSystem::Initialize(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto behaviours = pComponentManager->GetComponents<BehaviourComponent>(entity);

		for (auto behaviour : behaviours)
		{
			behaviour->Initialize();
		}
	}
}

void BehaviourSystem::Update(ComponentManager* const pComponentManager)
{
	size_t const size = m_ToBeAdded.size();
	for (size_t i = 0; i < size; i++)
	{
		auto behaviours = pComponentManager->GetComponents<BehaviourComponent>(m_ToBeAdded[i]);

		for (auto behaviour : behaviours)
		{
			if (behaviour->IsActive())
			{
				behaviour->Initialize();
			}
		}

		m_Entities.insert(m_ToBeAdded[i]);
	}

	m_ToBeAdded.erase(m_ToBeAdded.begin(), m_ToBeAdded.begin() + size);

	for (Entity const& entity : m_Entities)
	{
		auto behaviours = pComponentManager->GetComponents<BehaviourComponent>(entity);

		for (auto behaviour : behaviours)
		{
			if (!behaviour->IsActive())
			{
				continue;
			}

			behaviour->Update();
		}
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