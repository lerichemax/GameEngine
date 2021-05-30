#include "PCH.h"
#include "InputManager.h"
#include "Command.h"

#include <algorithm>

using namespace empire;

InputManager::InputManager()
	:m_pActions(),
	m_MousePosition(0,0),
	m_bIsMouseClicked(false)
{}

InputManager::~InputManager()
{
	for(auto pair :m_pActions)
	{
		SafeDelete(pair.second);
	}
	m_pActions.clear();
}

InputAction::~InputAction()
{
	SafeDelete(pCommand);
}

bool InputManager::ProcessInput()
{
	m_bIsMouseClicked = false;
	ProcessControllerInput();
	return ProcessKeyboardInput();
}

bool InputManager::IsPressed(ControllerButton button) const
{
	if (button == ControllerButton::None)
	{
		return false;
	}
	return m_CurrentControllerState.Gamepad.wButtons & (WORD)button;
}

bool InputManager::IsPressed(SDL_KeyCode keyCode) const
{
	return m_pKeyboardState[keyCode];
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
		auto actionIt = std::find_if(m_pActions.begin(), m_pActions.end(), 
			[&e](std::pair<int, InputAction*> pair)
			{
				return pair.second->keyBoardBtn == e.key.keysym.sym;
			});
		if (actionIt != m_pActions.end() && (*actionIt).second->isActive)
		{
			switch (e.type)
			{
			case SDL_KEYDOWN:
				if ((*actionIt).second->state == KeyActionState::held ||
					(*actionIt).second->state == KeyActionState::pressed && (*actionIt).second->GetLastKeyPosition() == LastKeyPosition::up)
				{
					(*actionIt).second->pCommand->Execute();
				}
				(*actionIt).second->SetLastKeyPosition(LastKeyPosition::down);
				break;
			case SDL_KEYUP:
				if ((*actionIt).second->state == KeyActionState::released)
				{
					(*actionIt).second->pCommand->Execute();
				}
				(*actionIt).second->SetLastKeyPosition(LastKeyPosition::up);
				break;
			}
		}
		switch (e.type) // Handle mouse events
		{
		case SDL_MOUSEMOTION:
			m_MousePosition = glm::vec2(e.motion.x, e.motion.y);
			break;
		case SDL_MOUSEBUTTONUP:
			m_bIsMouseClicked = true;
			break;
		}
	}
	SDL_PumpEvents();
	m_pKeyboardState = SDL_GetKeyboardState(NULL);
	
	return true;
}

void InputManager::ProcessControllerInput()
{
	
	ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentControllerState);

	std::for_each(m_pActions.begin(), m_pActions.end(), [this](std::pair<const int, InputAction*>& actionPair)
		{
			if (actionPair.second->gamepadBtn == ControllerButton::None || !actionPair.second->isActive)
			{
				return;
			}
		
			switch (actionPair.second->state)
			{
			case KeyActionState::held:
				if (IsPressed(static_cast<ControllerButton>(actionPair.second->gamepadBtn)))
				{
					actionPair.second->SetLastKeyPosition(LastKeyPosition::down);
					actionPair.second->pCommand->Execute();
				}
				actionPair.second->SetLastKeyPosition(LastKeyPosition::up);
				break;
			case KeyActionState::pressed:
				if (IsPressed(static_cast<ControllerButton>(actionPair.second->gamepadBtn)))
				{
					if (actionPair.second->GetLastKeyPosition() == LastKeyPosition::up)
					{
						actionPair.second->pCommand->Execute();
					}
					actionPair.second->SetLastKeyPosition(LastKeyPosition::down);
					break;
				}
				actionPair.second->SetLastKeyPosition(LastKeyPosition::up);
				break;
			case KeyActionState::released:
				if (!IsPressed(static_cast<ControllerButton>(actionPair.second->gamepadBtn)))
				{
					if (actionPair.second->GetLastKeyPosition() == LastKeyPosition::down)
					{
						actionPair.second->pCommand->Execute();
					}
					actionPair.second->SetLastKeyPosition(LastKeyPosition::up);
					break;
				}
				actionPair.second->SetLastKeyPosition(LastKeyPosition::down);
				break;
			}
		});
}

//void InputManager::AddCommand(SDL_Keycode keyCode, Command* pCommand)
//{
//	m_CommandMap.insert(std::make_pair(keyCode, pCommand));
//}

void InputManager::AddInputAction(int id, InputAction* pAction)
{
	if (m_pActions.find(id) != m_pActions.end())
	{
		Debugger::GetInstance().LogWarning("An Action already exists for this id");
		return;
	}
	m_pActions.insert(std::make_pair(id, pAction));
}

InputAction* InputManager::GetAction(int id)
{
	if (m_pActions.find(id) == m_pActions.end())
	{
		Debugger::GetInstance().LogWarning("No actions exists for this id");
		return nullptr;
	}
	
	return m_pActions.at(id);
}