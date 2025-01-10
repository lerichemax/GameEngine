#include "PCH.h"
#include "TextRendererSystem.h"

#include "TextRendererComponent.h"

#include "ResourceManager.h"

void TextRendererSystem::Initialize()
{
	auto view = m_pRegistry->GetView<TransformComponent, RendererComponent, TextRendererComponent>();
	for (Entity const& entity : view)
	{
		auto pRenderComp = m_pRegistry->GetComponent<RendererComponent>(entity);

		pRenderComp->Space = RenderSpace::Screen;
	}
}

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

		auto renderComp = m_pRegistry->GetComponent<RendererComponent>(entity);

		assert(textRenderComp->m_pFont != nullptr && "Font is null");

		renderComp->pTexture = ResourceManager::Get().GetTextTexture(textRenderComp->m_pFont->GetFont(), textRenderComp->m_Text.c_str(),
			textRenderComp->GetSDLColor(), textRenderComp->m_TextId);

		textRenderComp->m_NeedsUpdate = false;
	}
}
