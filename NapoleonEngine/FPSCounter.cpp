#include "PCH.h"
#include "FPSCounter.h"
#include "TextRendererComponent.h"
#include "ResourceManager.h"
#include "Timer.h"

void FPSCounter::Initialize()
{
	//m_pTextComp = m_pGameObject->GetComponent<TextRendererComponent>();
}

void FPSCounter::Update()
{
	m_pTextComp->SetText("FPS " + std::to_string(Timer::GetInstance().GetFPS()));
}

void FPSCounterSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto textRenderComp = pComponentManager->GetComponent<ECS_TextRendererComponent>(entity);

		if (!textRenderComp->IsActive())
		{
			continue;
		}

		textRenderComp->m_Text = "FPS " + std::to_string(Timer::GetInstance().GetFPS());
	}
}

void FPSCounterSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TextRendererComponent>());
	signature.set(pRegistry->GetComponentType<FPSCounterComponent>());

	pRegistry->SetSystemSignature<FPSCounterSystem>(signature);
}