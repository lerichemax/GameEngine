#include "PCH.h"
#include "SwitchTextColor.h"

#include "TextRendererComponent.h"

SwitchTextColor::SwitchTextColor(Color targetColor, TextRendererComponent* pText)
	:m_TargetColor(targetColor),
	m_pText(pText)
{
	
}

SwitchTextColor::SwitchTextColor(SwitchTextColor const& other)
	:m_TargetColor(other.m_TargetColor),
	m_pText(other.m_pText)
{
	
}

void SwitchTextColor::Execute()
{
	m_pText->SetTextColor(m_TargetColor.R, m_TargetColor.G, m_TargetColor.B);
}