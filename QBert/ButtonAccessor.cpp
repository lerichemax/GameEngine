#include "PCH.h"
#include "ButtonsAcessor.h"

#include "ButtonComponent.h"

ButtonAccessor::ButtonAccessor()
	:m_pButtons()
{
	
}

ButtonAccessor::ButtonAccessor(ButtonAccessor const& other)
	:m_pButtons()
{
	for (auto pair : other.m_pButtons)
	{
		m_pButtons.insert(std::make_pair(pair.first, pair.second->Clone()));
	}
}

void ButtonAccessor::AddButton(std::string const& key, ButtonComponent* pBtn)
{
	m_pButtons.insert(std::make_pair(key, pBtn));
}

void ButtonAccessor::RemoveButton(std::string const& key)
{
	if (m_pButtons.find(key) != m_pButtons.end())
	{
		m_pButtons.erase(key);
	}
}

ButtonComponent* ButtonAccessor::GetButton(std::string const& key)const
{
	if (m_pButtons.find(key) != m_pButtons.end())
	{
		return m_pButtons.at(key);
	}
	return nullptr;
}