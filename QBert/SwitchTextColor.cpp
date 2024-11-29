#include "PCH.h"
#include "SwitchTextColor.h"

#include "TextRendererComponent.h"

SwitchTextColor::SwitchTextColor()
	:m_pTargetColor{nullptr}
{

}

SwitchTextColor::SwitchTextColor(Color const& targetColor)
	:m_pTargetColor(new Color{targetColor})
{
	
}

void SwitchTextColor::Execute(GameObject const& gObject)
{
	auto pText = gObject.GetComponent<TextRendererComponent>();
	if (pText != nullptr)
	{
		pText->SetTextColor(m_pTargetColor->R, m_pTargetColor->G, m_pTargetColor->B);
	}
}