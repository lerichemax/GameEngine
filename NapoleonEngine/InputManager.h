#pragma once

#include "Singleton.h"

#include "SDL.h"
#include <XInput.h>
#include <memory>

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

enum class PlayerNbr // to remove
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
		playerNbr{},
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
	bool IsDown(ControllerButton button, PlayerNbr nbr) const;
	bool IsDown(SDL_Scancode keyCode) const;
	bool IsUp(SDL_Scancode keyCode) const;
	bool IsHeldDown(SDL_Scancode keyCode) const;
	void AddInputAction(InputAction* action);
		
	glm::vec2 GetMousePosition() const;
	bool IsLMBPressed() const;
	bool IsRMBPressed() const;
	
private:
	friend class Singleton<InputManager>;
		
	class InputManagerImpl;
	std::unique_ptr<InputManagerImpl> m_pImpl;
		
	InputManager();
};
