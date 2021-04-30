#include "PCH.h"
#include "FPSCounter.h"
#include "TextRendererComponent.h"
#include "ResourceManager.h"
#include "Timer.h"

void empire::FPSCounter::Initialize()
{
	m_pTextComp = m_pGameObject->GetComponent<TextRendererComponent>();
}

void empire::FPSCounter::Update()
{
	m_pTextComp->SetText("FPS " + std::to_string(int(1 / Timer::GetInstance().GetDeltaTime())));
}