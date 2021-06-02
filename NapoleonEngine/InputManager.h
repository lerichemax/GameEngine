#pragma once
#include <XInput.h>
#include "Singleton.h"

#include "SDL.h"
#include <map>

namespace empire
{
	class Command;
	enum class ControllerButton : WORD
	{
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y,
		ButtonUp = XINPUT_GAMEPAD_DPAD_UP,
		ButtonLeft = XINPUT_GAMEPAD_DPAD_LEFT,
		ButtonDown = XINPUT_GAMEPAD_DPAD_DOWN,
		ButtonRight = XINPUT_GAMEPAD_DPAD_RIGHT,
		LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		Back = XINPUT_GAMEPAD_BACK,
		Start = XINPUT_GAMEPAD_START,
		None = 65535
	};

	enum class PlayerNbr
	{
		One,
		Two,
		Three,
		Four
	};

	enum class KeyActionState
	{
		pressed,
		held,
		released
	};

	enum class LastKeyPosition
	{
		up, down
	};

	enum class MouseButton : int
	{
		left = SDL_BUTTON_LEFT,
		right = SDL_BUTTON_RIGHT,
		middle = SDL_BUTTON_MIDDLE,
		none = -1
	};
	
	struct InputAction
	{
	private:
		friend class InputManager;
		
		SDL_KeyCode keyBoardBtn;
		ControllerButton gamepadBtn;
		MouseButton mouseBtn;

		Command* pCommand;
		
		KeyActionState state;
		LastKeyPosition lastKeyPos;
		PlayerNbr playerNbr;
		
		bool isTriggered;
	
	public:
		InputAction(SDL_KeyCode keyboardBtn, KeyActionState state, Command* pCom)//keyboard action
			:keyBoardBtn(keyboardBtn),
			gamepadBtn(ControllerButton::None),
			mouseBtn(MouseButton::none),
			state(state),
			isTriggered(false),
			pCommand(pCom),
			lastKeyPos(LastKeyPosition::up)
		{
		}
		
		InputAction(ControllerButton gamepadBtn, KeyActionState state, Command* pCom, PlayerNbr nbr) //controller action
			:keyBoardBtn((SDL_KeyCode)-1),
			gamepadBtn(gamepadBtn),
			mouseBtn(MouseButton::none),
			state(state),
			isTriggered(false),
			pCommand(pCom),
			lastKeyPos(LastKeyPosition::up),
			playerNbr(nbr)
		{
		}

		InputAction(MouseButton mouseBtn, KeyActionState state, Command* pCom) //mouse action
			:keyBoardBtn((SDL_KeyCode)-1),
			gamepadBtn(ControllerButton::None),
			mouseBtn(mouseBtn),
			state(state),
			isTriggered(false),
			pCommand(pCom),
			lastKeyPos(LastKeyPosition::up)
		{
		}
		~InputAction();
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager(InputManager const& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(InputManager const& rhs) = delete;
		InputManager& operator=(InputManager&& rhs) = delete;
		~InputManager();
		
		bool ProcessInput();
		bool IsPressed(ControllerButton button, PlayerNbr nbr) const;
		bool IsPressed(SDL_KeyCode keyCode) const;
		bool IsActionTriggered(int id) const;
		void AddInputAction(int id, InputAction* pAction);
		InputAction* GetAction(int id);
		
		glm::vec2 GetMousePosition() const { return m_MousePosition; }
		bool IsMousePressed() const { return m_bIsMouseLBtnClicked; }
	
	private:
		friend class Singleton<InputManager>;
		InputManager();

		XINPUT_STATE m_CurrentControllerState[XUSER_MAX_COUNT];
		bool m_ConnectedControllers[XUSER_MAX_COUNT];
		
		Uint8 const*  m_pKeyboardState;
		
		std::map<int, InputAction*> m_pActions;
		
		glm::vec2 m_MousePosition;
		
		bool m_bIsMouseLBtnClicked;
		bool m_bIsMouseRBtnClicked;
		
		void ProcessControllerInput();
		bool ProcessSDLEvents();
		void ProcessKeyboardInput(SDL_KeyboardEvent const& e, InputAction* pCurrentAction);
		void ProcessMouseInput(SDL_Event const& e, InputAction* pCurrentAction);
		void RefreshControllerConnections();
		void ResetTriggeredState();

		void ProcessButtonDown(InputAction* pCurrentAction);
		void ProcessButtonUp(InputAction* pCurrentAction);
		
	};
}
