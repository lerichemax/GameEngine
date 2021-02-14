#include "MiniginPCH.h"


#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextObject::TextObject(const std::string& text, Font* const pFont) 
	: SceneObject{},
	m_NeedsUpdate(true),
	m_Text{text},
	m_pTxtRendererComponent{}
{
	auto pTxtComp{ new TextRendererComponent{ pFont } };
	AddComponent<TextRendererComponent>(pTxtComp);
	m_pTxtRendererComponent = pTxtComp;
}

void dae::TextObject::Update()
{
	if (m_NeedsUpdate)
	{
		m_pTxtRendererComponent->Update(m_Text);
		m_NeedsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextObject::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}