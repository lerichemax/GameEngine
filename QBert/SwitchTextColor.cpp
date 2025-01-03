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

SwitchTextColor::SwitchTextColor(SwitchTextColor const& other)
	:m_pTargetColor{ new Color{*other.m_pTargetColor} }
{

}

SwitchTextColor::SwitchTextColor(SwitchTextColor&& other)
	:m_pTargetColor{ new Color{*other.m_pTargetColor} }
{
	SAFE_DELETE(other.m_pTargetColor);
}

SwitchTextColor& SwitchTextColor::operator=(SwitchTextColor const& other)
{
	m_pTargetColor = new Color{ *other.m_pTargetColor };

	return *this;
}

SwitchTextColor& SwitchTextColor::operator=(SwitchTextColor&& other)
{
	m_pTargetColor = new Color{ *other.m_pTargetColor };

	SAFE_DELETE(other.m_pTargetColor);

	return *this;
}

SwitchTextColor::~SwitchTextColor()
{
	SAFE_DELETE(m_pTargetColor);
}

void SwitchTextColor::Execute(GameObject const& gObject)
{
	auto pText = gObject.GetComponent<TextRendererComponent>();
	if (pText != nullptr)
	{
		pText->SetTextColor(m_pTargetColor->R, m_pTargetColor->G, m_pTargetColor->B);
	}
}