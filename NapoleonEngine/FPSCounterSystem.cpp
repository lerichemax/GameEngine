#include "PCH.h"
#include "FPSCounterSystem.h"

#include "TextRendererComponent.h"
#include "FPSCounter.h"

#include "Timer.h"

void FPSCounterSystem::Update()
{
	for (Entity const& entity : m_Entities)
	{
		auto* const fpsCounter = m_pRegistry->GetComponent<FPSCounter>(entity);

		if (fpsCounter != nullptr && fpsCounter->IsActive())
		{
			fpsCounter->m_FPS = Timer::GetInstance().GetFPS();

			auto* const text = m_pRegistry->GetComponent<TextRendererComponent>(entity);

			if (text != nullptr && text->IsActive())
			{
				text->SetText(std::string{ "FPS " } + std::to_string(fpsCounter->m_FPS));
			}
		}
	}
}

void FPSCounterSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<FPSCounter>());
	signature.set(m_pRegistry->GetComponentType<TextRendererComponent>());

	m_pRegistry->SetSystemSignature<FPSCounterSystem>(signature);
}

void FPSCounterSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64(std::string{ "type" }, static_cast<int64_t>(std::type_index(typeid(FPSCounterSystem)).hash_code()));
}