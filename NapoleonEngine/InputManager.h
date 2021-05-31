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
	
	struct InputAction
	{
		int id;
		SDL_KeyCode keyBoardBtn;
		ControllerButton gamepadBtn;
		KeyActionState state;
		bool isActive;
		Command* pCommand;

		InputAction(SDL_KeyCode keyboardBtn, KeyActionState state, Command* pCom)//keyboard action
			:keyBoardBtn(keyboardBtn),
			gamepadBtn(ControllerButton::None),
			state(state),
			isActive(true),
			pCommand(pCom),
			lastKeyPos(LastKeyPosition::up)
		
		{
		}
		InputAction(ControllerButton gamepadBtn, KeyActionState state, Command* pCom) //controller action
			:keyBoardBtn((SDL_KeyCode)-1),
			gamepadBtn(gamepadBtn),
			state(state),
			isActive(true),
			pCommand(pCom),
			lastKeyPos(LastKeyPosition::up)
		{
		}
		~InputAction();
		
		LastKeyPosition GetLastKeyPosition() const { return lastKeyPos; }
		void SetLastKeyPosition(LastKeyPosition lkp) { lastKeyPos = lkp; }
	
	private:
		LastKeyPosition lastKeyPos;
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
		bool IsPressed(ControllerButton button) const;
		bool IsPressed(SDL_KeyCode keyCode) const;
		void AddInputAction(int id, InputAction* pAction);
		InputAction* GetAction(int id);
		
		glm::vec2 GetMousePosition() const { return m_MousePosition; }
		bool IsMousePressed() const { return m_bIsMouseClicked; }
	private:
		friend class Singleton<InputManager>;
		InputManager();

		
		XINPUT_STATE m_CurrentControllerState{};

		Uint8 const*  m_pKeyboardState;
		
		std::map<int, InputAction*> m_pActions;
		
		glm::vec2 m_MousePosition;
		
		bool m_bIsMouseClicked; //left button

		void ProcessControllerInput();
		bool ProcessKeyboardInput();
		
	};
}
