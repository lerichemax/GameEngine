#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"

using namespace dae;

InputManager::InputManager()
	:m_pLastKeyboardState{new BYTE[256]},
	m_pCurrentKeyboardState{new BYTE[256]}
{
	m_pLastKeyboardState = SDL_GetKeyboardState(NULL);
}

bool InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentControllerState);

	if (!ProcessControllerInput())
	{
		return false;
	}
	if(!ProcessKeyboardInput())
	{
		return false;
	}

	return true;
}

bool InputManager::IsPressed(ControllerButton button) const
{
	switch (button)
	{
	case ControllerButton::ButtonA:
		return m_CurrentControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	case ControllerButton::ButtonB:
		return m_CurrentControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
	case ControllerButton::ButtonX:
		return m_CurrentControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	case ControllerButton::ButtonY:
		return m_CurrentControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	default: return false;
	}
}

bool InputManager::ProcessKeyboardInput()
{
	m_pCurrentKeyboardState = SDL_GetKeyboardState(NULL);
	for (auto& pair : m_CommandMap)
	{
		if (pair.first > 0x07 && pair.first <= 0xFE)
		{
			if ((m_pCurrentKeyboardState[pair.first]) != 0)
			{
				switch (pair.second->GetKeyActionState())
				{
				case KeyActionState::pressed:
					if ((m_pLastKeyboardState[pair.first]) == 0)
					{
						if (!pair.second->Execute())
						{
							return false;
						}
						
					}
					break;
				case KeyActionState::held:
					if (!pair.second->Execute())
					{
						return false;
					}
					break;
				}
			}
			else //released
			{
				if (pair.second->GetKeyActionState() == KeyActionState::released)
				{
					if ((m_pLastKeyboardState[pair.first]) != 0)
					{
						if (!pair.second->Execute())
						{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

bool InputManager::ProcessControllerInput()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);
	int key{ state.Gamepad.wButtons };

	//Pressed + Held
	if (m_CommandMap.find(key) != m_CommandMap.end())
	{
		switch (m_CommandMap.at(key)->GetKeyActionState())
		{
		case KeyActionState::pressed:
			if (m_CurrentControllerState.Gamepad.wButtons != key)
			{
				if (!m_CommandMap.at(key)->Execute())
				{
					return false;
				}
			}
			break;
		case KeyActionState::held:
			if (!m_CommandMap.at(key)->Execute())
			{
				return false;
			}
			break;
		}
	}

	//Released
	if (m_CommandMap.find(m_CurrentControllerState.Gamepad.wButtons) != m_CommandMap.end()
		&& m_CommandMap.at(m_CurrentControllerState.Gamepad.wButtons)->GetKeyActionState() == KeyActionState::released)
	{
		if (m_CurrentControllerState.Gamepad.wButtons != key)
		{
			if(!m_CommandMap.at(m_CurrentControllerState.Gamepad.wButtons)->Execute())
			{
				return false;
			}
		}
	}

	m_CurrentControllerState = state;
	return true;
}

void InputManager::AddCommand(SDL_Keycode keyCode, Command* pCommand)
{
	m_CommandMap.insert(std::make_pair(keyCode, pCommand));
}