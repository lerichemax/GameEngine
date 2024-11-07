#include "PCH.h"
#include "TextRendererSystem.h"

#include "TextRendererComponent.h"

#include "ResourceManager.h"

void TextRendererSystem::Update()
{
	auto view = m_pRegistry->GetView<TransformComponent, RendererComponent, TextRendererComponent>();
	for (Entity const& entity : view)
	{
		auto textRenderComp = m_pRegistry->GetComponent<TextRendererComponent>(entity);


		if (!textRenderComp->IsActive() || !textRenderComp->m_NeedsUpdate)
		{
			continue;
		}

		auto renderComp = m_pRegistry->GetComponent<RendererComponent>(entity); //log if absent (also when adding)

		renderComp->pTexture = ResourceManager::Get().GetTextTexture(textRenderComp->m_pFont->GetFont(), textRenderComp->m_Text.c_str(),
			textRenderComp->m_TextColor, textRenderComp->m_TextId);

		textRenderComp->m_NeedsUpdate = false;
	}
}
