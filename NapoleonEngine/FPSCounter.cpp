#include "PCH.h"
#include "FPSCounter.h"
#include "TextRendererComponent.h"
#include "ResourceManager.h"
#include "Timer.h"

void FPSCounter::Serialize(StreamWriter& writer) const
{ 
	writer.WriteString("type", typeid(FPSCounter).name()); 
	
	BehaviourComponent::Serialize(writer);
};

void FPSCounter::Update()
{
	auto textRendererComp = GetGameObject()->GetComponent<TextRendererComponent>();

	if (textRendererComp != nullptr)
	{
		textRendererComp->m_Text = "FPS " + std::to_string(Timer::GetInstance().GetFPS());
	}
}