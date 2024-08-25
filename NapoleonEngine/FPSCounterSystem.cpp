#include "PCH.h"
#include "FPSCounterSystem.h"

#include "TextRendererComponent.h"
#include "FPSCounter.h"

#include "Timer.h"

void FPSCounterSystem::Update()
{
	for (Entity const& entity : m_Entities)
	{
		auto* const fpsCounter = m_pCompManager->GetComponent<FPSCounter>(entity);

		if (fpsCounter != nullptr && fpsCounter->IsActive())
		{
			fpsCounter->m_FPS = Timer::GetInstance().GetFPS();

			auto* const text = m_pCompManager->GetComponent<TextRendererComponent>(entity);

			if (text != nullptr && text->IsActive())
			{
				text->SetText("FPS " + std::to_string(fpsCounter->m_FPS));
			}
		}
	}
}

void FPSCounterSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<FPSCounter>());
	signature.set(pRegistry->GetComponentType<TextRendererComponent>());

	pRegistry->SetSystemSignature<FPSCounterSystem>(signature);
}

void FPSCounterSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(FPSCounterSystem).name());
}