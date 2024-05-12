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

		textRenderComp->m_Text = "FPS " + std::to_string(Timer::GetInstance().GetFPS());

	}
}