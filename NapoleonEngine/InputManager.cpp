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
	bool IsDown(ControllerButton button, PlayerNbr nbr) const;
	bool IsDown(SDL_Scancode keyCode) const;
	bool IsUp(SDL_Scancode keyCode) const;
	bool IsHeldDown(SDL_Scancode keyCode) const;

	Command* const GetCommand(SDL_KeyCode keyCode) const;
	Command* const GetCommand(ControllerButton ctrlBtn) const;
	Command* const GetCommand(MouseButton mousebtn) const;
	void HandleInput(SDL_KeyCode keyCode, GameObject* const gObject);
	void HandleInput(ControllerButton ctrlBtn, GameObject* const gObject);
	void HandleInput(MouseButton mousebtn, GameObject* const gObject);

	glm::vec2 GetMousePosition() const { return m_MousePosition; }
	bool IsLMBPressed() const { return m_bIsMouseLBtnClicked; }
	bool IsRMBPressed() const { return m_bIsMouseRBtnClicked; }

	void AddInputAction(InputAction* action);

private:
	XINPUT_STATE m_CurrentControllerState[XUSER_MAX_COUNT];
	bool m_ConnectedControllers[XUSER_MAX_COUNT];

	Uint8 const* m_pKeyboardState;
	Uint8* m_pPreviousKeyboardState;

	int m_KeyboardLength;

	std::vector<InputAction*> m_pActions;

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
	m_pKeyboardState(SDL_GetKeyboardState(&m_KeyboardLength)),
	m_pPreviousKeyboardState(new Uint8[m_KeyboardLength]),
	m_CurrentControllerState()
{
	RefreshControllerConnections();
}

InputManager::InputManagerImpl::~InputManagerImpl()
{
	for (InputAction* action : m_pActions)
	{
		SafeDelete(action);
	}
	m_pActions.clear();

	SafeDelete(m_pPreviousKeyboardState);
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
	for (InputAction* action : m_pActions)
	{
		if ((action->state == KeyActionState::released && action->isTriggered) ||
			action->state == KeyActionState::pressed && action->isTriggered)
		{
			action->isTriggered = false;
		}
	}
}

bool InputManager::InputManagerImpl::IsDown(ControllerButton button, PlayerNbr nbr) const
{
	if (button == ControllerButton::None)
	{
		return false;
	}
	return m_CurrentControllerState[(int)nbr].Gamepad.wButtons & (WORD)button;
}

bool InputManager::InputManagerImpl::IsDown(SDL_Scancode keyCode) const
{
	return !m_pPreviousKeyboardState[keyCode] && m_pKeyboardState[keyCode];
}

bool InputManager::InputManagerImpl::IsUp(SDL_Scancode keyCode) const
{
	return m_pPreviousKeyboardState[keyCode] && !m_pKeyboardState[keyCode];
}

bool InputManager::InputManagerImpl::IsHeldDown(SDL_Scancode keyCode) const
{
	return m_pPreviousKeyboardState[keyCode] && m_pKeyboardState[keyCode];
}

Command* const InputManager::InputManagerImpl::GetCommand(SDL_KeyCode keyCode) const
{
	auto action = std::find_if(m_pActions.begin(), m_pActions.end(), [keyCode](InputAction* action) {
		return action->keyBoardBtn == keyCode && action->isTriggered;
		});

	if (action != m_pActions.end())
	{
		return (*action)->pCommand;
	}

	return nullptr;
}
Command* const InputManager::InputManagerImpl::GetCommand(ControllerButton ctrlBtn) const
{
	auto action = std::find_if(m_pActions.begin(), m_pActions.end(), [ctrlBtn](InputAction* action) {
		return action->gamepadBtn == ctrlBtn && action->isTriggered;
		});

	if (action != m_pActions.end())
	{
		return (*action)->pCommand;
	}

	return nullptr;
}
Command* const InputManager::InputManagerImpl::GetCommand(MouseButton mousebtn) const
{
	auto action = std::find_if(m_pActions.begin(), m_pActions.end(), [mousebtn](InputAction* action) {
		return action->mouseBtn == mousebtn && action->isTriggered;
		});

	if (action != m_pActions.end())
	{
		return (*action)->pCommand;
	}

	return nullptr;
}
void InputManager::InputManagerImpl::HandleInput(SDL_KeyCode keyCode, GameObject* const gObject)
{
	auto cmd = GetCommand(keyCode);

	if (cmd != nullptr)
	{
		cmd->Execute(gObject);
	}
}
void InputManager::InputManagerImpl::HandleInput(ControllerButton ctrlBtn, GameObject* const gObject)
{
	auto cmd = GetCommand(ctrlBtn);

	if (cmd != nullptr)
	{
		cmd->Execute(gObject);
	}
}
void InputManager::InputManagerImpl::HandleInput(MouseButton mousebtn, GameObject* const gObject)
{
	auto cmd = GetCommand(mousebtn);

	if (cmd != nullptr)
	{
		cmd->Execute(gObject);
	}
}

bool InputManager::InputManagerImpl::ProcessSDLEvents()
{
	memcpy(m_pPreviousKeyboardState, m_pKeyboardState, m_KeyboardLength);

	SDL_Event e;	
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		
		auto actionIt = std::find_if(m_pActions.begin(), m_pActions.end(), 
			[&e](InputAction* action)
			{
				return action->keyBoardBtn == e.key.keysym.sym;
			});

		std::for_each(m_pActions.begin(), m_pActions.end(), [&e, this](InputAction* action)
			{
				if (action->keyBoardBtn == e.key.keysym.sym)
				{
					ProcessKeyboardInput(e.key, action);

				}
				else if (action->mouseBtn != MouseButton::none)
				{
					ProcessMouseInput(e, action);
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
		pCurrentAction->isTriggered = true;
	}
	else
	{
		pCurrentAction->isTriggered = false;
	}
	pCurrentAction->lastKeyPos = LastKeyPosition::up;
}

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
	
	

	std::for_each(m_pActions.begin(), m_pActions.end(), [this](InputAction* pAction)
		{
			if (pAction->gamepadBtn == ControllerButton::None)
			{
				return;
			}
			
			switch (pAction->state)
			{
			case KeyActionState::held:
				if (IsDown(static_cast<ControllerButton>(pAction->gamepadBtn), pAction->playerNbr))
				{
					pAction->lastKeyPos = LastKeyPosition::down;
					pAction->isTriggered = true;
				}
				else
				{
					pAction->isTriggered = false;
				}
				pAction->lastKeyPos  = LastKeyPosition::up;
				break;
			case KeyActionState::pressed:
				if (IsDown(static_cast<ControllerButton>(pAction->gamepadBtn), pAction->playerNbr))
				{
					if (pAction->lastKeyPos == LastKeyPosition::up)
					{
						pAction->isTriggered = true;
					}
					
					pAction->lastKeyPos = LastKeyPosition::down;
					break;
				}
				pAction->lastKeyPos = LastKeyPosition::up;
				break;
			case KeyActionState::released:
				if (!IsDown(static_cast<ControllerButton>(pAction->gamepadBtn), pAction->playerNbr))
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

void InputManager::InputManagerImpl::AddInputAction(InputAction* action)
{
	m_pActions.push_back(action);
}

InputManager::InputManager()
	:Singleton<InputManager>(),
	m_pImpl(new InputManagerImpl())
{
}

InputManager::~InputManager()
{
}

bool InputManager::ProcessInput()
{
	return m_pImpl->ProcessInput();
}

bool InputManager::IsDown(ControllerButton button, PlayerNbr nbr) const
{
	return m_pImpl->IsDown(button, nbr);
}

bool InputManager::IsDown(SDL_Scancode keyCode) const
{
	return m_pImpl->IsDown(keyCode);
}

bool InputManager::IsUp(SDL_Scancode keyCode) const
{
	return m_pImpl->IsUp(keyCode);
}

bool InputManager::IsHeldDown(SDL_Scancode keyCode) const
{
	return m_pImpl->IsHeldDown(keyCode);
}

void InputManager::AddInputAction(InputAction* action)
{
	m_pImpl->AddInputAction(action);
}

Command* const InputManager::GetCommand(SDL_KeyCode keyCode) const
{
	return m_pImpl->GetCommand(keyCode);
}

Command* const InputManager::GetCommand(ControllerButton ctrlBtn) const
{
	return m_pImpl->GetCommand(ctrlBtn);
}

Command* const InputManager::GetCommand(MouseButton mousebtn) const
{
	return m_pImpl->GetCommand(mousebtn);
}

void InputManager::HandleInput(SDL_KeyCode keyCode, GameObject* const gObject)
{
	m_pImpl->HandleInput(keyCode, gObject);
}

void InputManager::HandleInput(ControllerButton ctrlBtn, GameObject* const gObject)
{
	m_pImpl->HandleInput(ctrlBtn, gObject);
}

void InputManager::HandleInput(MouseButton mousebtn, GameObject* const gObject)
{
	m_pImpl->HandleInput(mousebtn, gObject);
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
