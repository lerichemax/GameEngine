#include "PCH.h"
#include "AudioSystem.h"
#include "SoundEffect.h"
#include "AudioComponent.h"
#include "Coordinator.h"
#include "ResourceManager.h"	

#include <algorithm>
#include <thread>


void AudioSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto audio = pComponentManager->GetComponent<AudioComponent>(entity);

		if (!audio->IsActive())
		{
			continue;
		}

		if (audio->m_Play)
		{
			ResourceManager::GetInstance().GetEffectById(audio->m_AudioId)->Play(audio->m_Volume);
			if (!audio->m_Loop)
			{
				audio->m_Play = false;
			}
		}
	}
}

void AudioSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<AudioComponent>());

	pRegistry->SetSystemSignature<AudioSystem>(signature);
}