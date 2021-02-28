#include "MiniginPCH.h"
#include "InputManager.h"
#include "Command.h"

using namespace dae;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
	for (auto pair : m_CommandMap)
	{
		delete pair.second;
	}
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
	SDL_Event e;
	for (auto& pair : m_CommandMap)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.key.keysym.sym == pair.first)
			{
				switch (pair.second->GetKeyActionState())
				{
				case KeyActionState::pressed:
					if (e.type == SDL_KEYDOWN)
					{
						if (pair.second->GetLastKeyState() == LastKeyState::up)
						{
							pair.second->SetLastKeyState(LastKeyState::down);
							if (!pair.second->Execute())
							{
								return false;
							}
						}
					}
					else
					{
						pair.second->SetLastKeyState(LastKeyState::up);
					}
					break;
				case KeyActionState::held:
					if (e.type == SDL_KEYDOWN)
					{
						if (!pair.second->Execute())
						{
							return false;
						}
					}
					else
					{
						pair.second->SetLastKeyState(LastKeyState::up);
					}
					break;
				case KeyActionState::released:
					if (e.type == SDL_KEYUP)
					{
						pair.second->SetLastKeyState(LastKeyState::up);
						if (!pair.second->Execute())
						{
							return false;
						}
					}
					else
					{
						pair.second->SetLastKeyState(LastKeyState::down);
					}
					break;
				}
			}
			else if (e.type == SDL_QUIT)
			{
				return false;
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