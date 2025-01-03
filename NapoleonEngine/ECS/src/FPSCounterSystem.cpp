#include "PCH.h"
#include "FPSCounterSystem.h"

#include "TextRendererComponent.h"
#include "FPSCounter.h"

#include "Timer.h"

void FPSCounterSystem::Update()
{
	auto view = m_pRegistry->GetView<FPSCounter, TextRendererComponent>();
	for (Entity entity : view)
	{
		auto* const fpsCounter = m_pRegistry->GetComponent<FPSCounter>(entity);

		if (fpsCounter != nullptr && fpsCounter->IsActive())
		{
			fpsCounter->m_FPS = TimerLocator::Get()->GetFPS();

			auto* const text = m_pRegistry->GetComponent<TextRendererComponent>(entity);

			if (text != nullptr && text->IsActive())
			{
				text->SetText(std::string{ "FPS " } + std::to_string(fpsCounter->m_FPS));
			}
		}
	}
}
