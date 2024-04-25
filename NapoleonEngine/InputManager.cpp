#include "PCH.h"
#include "InputManager.h"
#include "Command.h"

#include <algorithm>
#include <map>

InputAction::~InputAction()
{
	SafeDelete(pCommand);
}

class InputManager::InputManagerImpl final
{
public:
	InputManagerImpl();
	InputManagerImpl(InputManager const& other) = delete;
	InputManagerImpl(InputManagerImpl&& other) = delete;
	InputManagerImpl& operator=(InputManagerImpl const& rhs) = delete;
	InputManagerImpl& operator=(InputManagerImpl&& rhs) = delete;
	~InputManagerImpl();
	
	bool ProcessInput();
	bool IsPressed(ControllerButton button, PlayerNbr nbr) const;
	bool IsPressed(SDL_KeyCode keyCode) const;
	bool IsActionTriggered(int id) const;
	void AddInputAction(int id, InputAction* pAction);
	InputAction* GetAction(int id);

	glm::vec2 GetMousePosition() const { return m_MousePosition; }
	bool IsLMBPressed() const { return m_bIsMouseLBtnClicked; }
	bool IsRMBPressed() const { return m_bIsMouseRBtnClicked; }

private:
	XINPUT_STATE m_CurrentControllerState[XUSER_MAX_COUNT];
	bool m_ConnectedControllers[XUSER_MAX_COUNT];

	Uint8 const* m_pKeyboardState;

	std::map<int, InputAction*> m_pActions;

	glm::vec2 m_MousePosition;

	bool m_bIsMouseLBtnClicked;
	bool m_bIsMouseRBtnClicked;

	void RefreshControllerConnections();
	void ResetTriggeredState();
	
	void ProcessKeyboardInput(SDL_KeyboardEvent const& e, InputAction* pCurrentAction);
	void ProcessMouseInput(SDL_Event const& e, InputAction* pCurrentAction);
	void ProcessButtonDown(InputAction* pCurrentAction);
	void ProcessButtonUp(InputAction* pCurrentAction);
	void ProcessControllerInput();
	bool ProcessSDLEvents();
};


InputManager::InputManagerImpl::InputManagerImpl()
	:m_pActions(),
	m_MousePosition(0, 0),
	m_bIsMouseLBtnClicked(false),
	m_pKeyboardState(),
	m_CurrentControllerState()
{
	RefreshControllerConnections();
}

InputManager::InputManagerImpl::~InputManagerImpl()
{
	for (auto pair : m_pActions)
	{
		SafeDelete(pair.second);
	}
	m_pActions.clear();
}

InputManager::InputManager()
	:Singleton<InputManager>(),
	m_pImpl(new InputManagerImpl())
{
}

InputManager::~InputManager()
{
}

bool InputManager::InputManagerImpl::ProcessInput()
{
	RefreshControllerConnections();
	
	m_bIsMouseLBtnClicked = false;
	m_bIsMouseRBtnClicked = false;
	ResetTriggeredState();
	ProcessControllerInput();
	return ProcessSDLEvents();
}

void InputManager::InputManagerImpl::ResetTriggeredState()
{
	for (auto pActionPair : m_pActions)
	{
		if ((pActionPair.second->state == KeyActionState::released && pActionPair.second->isTriggered) ||
			pActionPair.second->state == KeyActionState::pressed && pActionPair.second->isTriggered)
		{
			pActionPair.second->isTriggered = false;
		}
	}
}

bool InputManager::InputManagerImpl::IsPressed(ControllerButton button, PlayerNbr nbr) const
{
	if (button == ControllerButton::None)
	{
		return false;
	}
	return m_CurrentControllerState[(int)nbr].Gamepad.wButtons & (WORD)button;
}

bool InputManager::InputManagerImpl::IsPressed(SDL_KeyCode keyCode) const
{
	return m_pKeyboardState[keyCode];
}

bool InputManager::InputManagerImpl::IsActionTriggered(int id) const
{
	if (m_pActions.find(id) == m_pActions.end())
	{
		return false;
	}

	return m_pActions.at(id)->isTriggered;
}

bool InputManager::InputManagerImpl::ProcessSDLEvents()
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

		std::for_each(m_pActions.begin(), m_pActions.end(), [&e, this](std::pair<int, InputAction*> pair)
			{
				auto pAction = pair.second;
				if (pAction->keyBoardBtn == e.key.keysym.sym)
				{
					ProcessKeyboardInput(e.key, pAction);

				}
				else if (pAction->mouseBtn != MouseButton::none)
				{
					ProcessMouseInput(e, pAction);
				}
			});

		switch (e.type) // Handle mouse events
		{
		case SDL_MOUSEMOTION:
			m_MousePosition = glm::vec2(e.motion.x, e.motion.y);
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				m_bIsMouseLBtnClicked = true;
			}
			else if (e.button.button == SDL_BUTTON_RIGHT)
			{
				m_bIsMouseRBtnClicked = true;
			}
			break;
		}
	}
	SDL_PumpEvents();
	m_pKeyboardState = SDL_GetKeyboardState(NULL);

	return true;
}

void InputManager::InputManagerImpl::ProcessKeyboardInput(SDL_KeyboardEvent const& e, InputAction* pCurrentAction)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		ProcessButtonDown(pCurrentAction);
		break;
	case SDL_KEYUP:
		ProcessButtonUp(pCurrentAction);
		break;
	}
}

void InputManager::InputManagerImpl::ProcessMouseInput(SDL_Event const& e, InputAction* pCurrentAction)
{
	if (pCurrentAction->mouseBtn == MouseButton::none)
	{
		return;
	}
	switch (e.type) // Handle mouse events
	{
	case SDL_MOUSEBUTTONUP:
		ProcessButtonUp(pCurrentAction);
		break;
	case SDL_MOUSEBUTTONDOWN:
		ProcessButtonDown(pCurrentAction);
		break;
	}
}

void InputManager::InputManagerImpl::ProcessButtonDown(InputAction* pCurrentAction)
{
	if (pCurrentAction->state == KeyActionState::held ||
		pCurrentAction->state == KeyActionState::pressed && pCurrentAction->lastKeyPos == LastKeyPosition::up)
	{

		if (pCurrentAction->pCommand != nullptr)
		{
			pCurrentAction->pCommand->Execute();
		}
		pCurrentAction->isTriggered = true;
	}
	else if (pCurrentAction->state == KeyActionState::released ||
		pCurrentAction->state == KeyActionState::pressed && pCurrentAction->lastKeyPos == LastKeyPosition::down)
	{
		pCurrentAction->isTriggered = false;
	}
	pCurrentAction->lastKeyPos = LastKeyPosition::down;
}

void InputManager::InputManagerImpl::ProcessButtonUp(InputAction* pCurrentAction)
{
	if (pCurrentAction->state == KeyActionState::released)
	{
		if (pCurrentAction->pCommand != nullptr)
		{
			pCurrentAction->pCommand->Execute();
		}
		pCurrentAction->isTriggered = true;
	}
	else
	{
		pCurrentAction->isTriggered = false;
	}
	pCurrentAction->lastKeyPos = LastKeyPosition::up;
}

/// <summary>
/// Copyright : DAE Overlord Engine
/// </summary>
void InputManager::InputManagerImpl::RefreshControllerConnections()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		const DWORD dwResult = XInputGetState(i, &state);
		m_ConnectedControllers[i] = (dwResult == ERROR_SUCCESS);
	}
}

void InputManager::InputManagerImpl::ProcessControllerInput()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if (!m_ConnectedControllers[i])
		{
			continue;
		}
		ZeroMemory(&m_CurrentControllerState[i], sizeof(XINPUT_STATE));
		XInputGetState(0, &m_CurrentControllerState[i]);
	}
	
	

	std::for_each(m_pActions.begin(), m_pActions.end(), [this](std::pair<const int, InputAction*>& actionPair)
		{
			auto pAction = actionPair.second;
			if (pAction->gamepadBtn == ControllerButton::None)
			{
				return;
			}
			
			switch (pAction->state)
			{
			case KeyActionState::held:
				if (IsPressed(static_cast<ControllerButton>(pAction->gamepadBtn), pAction->playerNbr))
				{
					pAction->lastKeyPos = LastKeyPosition::down;
					if (pAction->pCommand != nullptr)
					{
						pAction->pCommand->Execute();
					}
					pAction->isTriggered = true;
				}
				else
				{
					pAction->isTriggered = false;
				}
				pAction->lastKeyPos  = LastKeyPosition::up;
				break;
			case KeyActionState::pressed:
				if (IsPressed(static_cast<ControllerButton>(pAction->gamepadBtn), pAction->playerNbr))
				{
					if (pAction->lastKeyPos == LastKeyPosition::up)
					{
						if (pAction->pCommand != nullptr)
						{
							pAction->pCommand->Execute();
						}
						pAction->isTriggered = true;
					}
					
					pAction->lastKeyPos = LastKeyPosition::down;
					break;
				}
				pAction->lastKeyPos = LastKeyPosition::up;
				break;
			case KeyActionState::released:
				if (!IsPressed(static_cast<ControllerButton>(pAction->gamepadBtn), pAction->playerNbr))
				{
					if (pAction->lastKeyPos == LastKeyPosition::down)
					{
						pAction->isTriggered = true;
					}
					pAction->lastKeyPos = LastKeyPosition::up;
					break;
				}
				pAction->lastKeyPos = LastKeyPosition::down;
				break;
			}
		});
}

void InputManager::InputManagerImpl::AddInputAction(int id, InputAction* pAction)
{
	if (m_pActions.find(id) != m_pActions.end())
	{
		Debugger::GetInstance().LogWarning("An Action already exists for this id");
		return;
	}
	m_pActions.insert(std::make_pair(id, pAction));
}

InputAction* InputManager::InputManagerImpl::GetAction(int id)
{
	if (m_pActions.find(id) == m_pActions.end())
	{
		Debugger::GetInstance().LogWarning("No actions exists for this id");
		return nullptr;
	}
	
	return m_pActions.at(id);
}

bool InputManager::ProcessInput()
{
	return m_pImpl->ProcessInput();
}

bool InputManager::IsPressed(ControllerButton button, PlayerNbr nbr) const
{
	return m_pImpl->IsPressed(button, nbr);
}

bool InputManager::IsPressed(SDL_KeyCode keyCode) const
{
	return m_pImpl->IsPressed(keyCode);
}

bool InputManager::IsActionTriggered(int id) const
{
	return m_pImpl->IsActionTriggered(id);
}

void InputManager::AddInputAction(int id, InputAction* pAction)
{
	m_pImpl->AddInputAction(id, pAction);
}

InputAction* InputManager::GetAction(int id)
{
	return m_pImpl->GetAction(id);
}

glm::vec2 InputManager::GetMousePosition() const
{
	return m_pImpl->GetMousePosition();
}

bool InputManager::IsLMBPressed() const
{
	return m_pImpl->IsLMBPressed();
}

bool InputManager::IsRMBPressed() const
{
	return m_pImpl->IsRMBPressed();
}
