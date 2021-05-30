#include "PCH.h"
#include "CharacterInputManager.h"

#include "InputManager.h"

CharacterInputManager::CharacterInputManager()
	:m_bIsReceivingInput(),
	m_InputActionIds()
{
}

CharacterInputManager::CharacterInputManager(CharacterInputManager const& other)
	:m_bIsReceivingInput(other.m_bIsReceivingInput),
	m_InputActionIds(other.m_InputActionIds)
{
}

void CharacterInputManager::SetInputActionsActive(bool isActive)
{
	auto& pInputManager = empire::InputManager::GetInstance();
	empire::InputAction* pAction{nullptr};
	for (int id : m_InputActionIds)
	{
		pAction = pInputManager.GetAction(id);
		if (pAction != nullptr)
		{
			pAction->isActive = isActive;
		}
	}
}

void CharacterInputManager::AddInputId(int id)
{
	m_InputActionIds.push_back(id);
}