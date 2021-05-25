#include "PCH.h"
#include "InputManager.h"
#include "Command.h"

#include <algorithm>

using namespace empire;

InputManager::~InputManager()
{
	for (auto pair : m_CommandMap)
	{
		delete pair.second;
	}
}

bool InputManager::ProcessInput()
{
	if (m_bUseKeyboard)
	{
		return ProcessKeyboardInput();
	}
	
	return ProcessControllerInput();
}

bool InputManager::IsPressed(ControllerButton button) const
{
	return m_CurrentControllerState.Gamepad.wButtons & static_cast<int>(button);
}

bool InputManager::ProcessKeyboardInput()
{
	SDL_Event e;	
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if (m_CommandMap.find(e.key.keysym.sym) != m_CommandMap.end())
		{
			auto cmdPair = m_CommandMap.at(e.key.keysym.sym);
			switch (e.type)
			{
			case SDL_KEYDOWN:
				if (cmdPair->GetKeyActionState() == KeyActionState::held ||
					(cmdPair->GetKeyActionState() == KeyActionState::pressed && cmdPair->GetLastKeyPosition() == LastKeyPosition::up))
				{
					cmdPair->Execute();
				}
				cmdPair->SetLastKeyPosition(LastKeyPosition::down);
				break;
			case SDL_KEYUP:
				if (cmdPair->GetKeyActionState() == KeyActionState::released)
				{
					cmdPair->Execute();
				}
				cmdPair->SetLastKeyPosition(LastKeyPosition::up);
				break;
			}
		}
	}
	return true;
}

bool InputManager::ProcessControllerInput()
{
	
	//ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentControllerState);

	std::for_each(m_CommandMap.begin(), m_CommandMap.end(), [this](std::pair<const int, Command*>& cmdPair)
		{
			switch (cmdPair.second->GetKeyActionState())
			{
			case KeyActionState::held:
				if (IsPressed(static_cast<ControllerButton>(cmdPair.first)))
				{
					cmdPair.second->SetLastKeyPosition(LastKeyPosition::down);
					cmdPair.second->Execute();
				}
				cmdPair.second->SetLastKeyPosition(LastKeyPosition::up);
				break;
			case KeyActionState::pressed:
				if (IsPressed(static_cast<ControllerButton>(cmdPair.first)))
				{
					if (cmdPair.second->GetLastKeyPosition() == LastKeyPosition::up)
					{
						cmdPair.second->Execute();
					}
					cmdPair.second->SetLastKeyPosition(LastKeyPosition::down);
					break;
				}
				cmdPair.second->SetLastKeyPosition(LastKeyPosition::up);
				break;
			case KeyActionState::released:
				if (!IsPressed(static_cast<ControllerButton>(cmdPair.first)))
				{
					if (cmdPair.second->GetLastKeyPosition() == LastKeyPosition::down)
					{
						cmdPair.second->Execute();
					}
					cmdPair.second->SetLastKeyPosition(LastKeyPosition::up);
					break;
				}
				cmdPair.second->SetLastKeyPosition(LastKeyPosition::down);
				break;
			}
		});
	return true;
}

void InputManager::AddCommand(SDL_Keycode keyCode, Command* pCommand)
{
	m_CommandMap.insert(std::make_pair(keyCode, pCommand));
}