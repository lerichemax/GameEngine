#include "PCH.h"
#include "BehaviourSystem.h"
#include "BehaviourComponent.h"

void BehaviourSystem::Initialize()
{

}

void BehaviourSystem::Update()
{
	UpdateToBeStartedVector(m_pCompManager);

	for (Entity const& entity : m_Entities)
	{
		auto behaviours = m_pCompManager->GetComponents<BehaviourComponent>(entity);

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
	System::AddEntity(entity);
	m_ToBeStarted.push_back(entity);
}

void BehaviourSystem::UpdateToBeStartedVector(ComponentManager* const pComponentManager)
{
	size_t const size = m_ToBeStarted.size();

	for (size_t i = 0; i < size; i++)
	{
		auto behaviours = pComponentManager->GetComponents<BehaviourComponent>(m_ToBeStarted[i]);

		for (auto behaviour : behaviours)
		{
			if (behaviour->IsActive())
			{
				behaviour->Start();
			}
		}
	}

	m_ToBeStarted.erase(m_ToBeStarted.begin(), m_ToBeStarted.begin() + size);
}