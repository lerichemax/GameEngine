#include "PCH.h"
#include "TextRendererSystem.h"

#include "TextRendererComponent.h"

#include "ResourceManager.h"

void TextRendererSystem::Update()
{
	for (Entity const& entity : m_Entities)
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

void TextRendererSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());
	signature.set(m_pRegistry->GetComponentType<RendererComponent>());
	signature.set(m_pRegistry->GetComponentType<TextRendererComponent>());

	m_pRegistry->SetSystemSignature<TextRendererSystem>(signature);
}