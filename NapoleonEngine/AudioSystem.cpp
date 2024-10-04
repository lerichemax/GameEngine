#include "PCH.h"
#include "AudioSystem.h"
#include "SoundEffect.h"
#include "AudioComponent.h"
#include "Registry.h"
#include "ResourceManager.h"	

#include <algorithm>
#include <thread>


void AudioSystem::Update()
{
	for (Entity const& entity : m_Entities)
	{
		auto audioComps = m_pRegistry->GetComponents<AudioComponent>(entity);
		for (AudioComponent* const audio : audioComps)
		{
			if (!audio->IsActive())
			{
				continue;
			}

			if (audio->m_Play)
			{
				ResourceManager::Get().GetEffectById(audio->m_AudioId)->Play(audio->m_Volume);
				if (!audio->m_Loop)
				{
					audio->m_Play = false;
				}
			}
		}
	}
}

void AudioSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<AudioComponent>());

	m_pRegistry->SetSystemSignature<AudioSystem>(signature);
}