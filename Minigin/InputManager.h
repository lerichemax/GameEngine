#pragma once
#include <XInput.h>
#include "Singleton.h"

#include <SDL.h>
#include <map>
#include <SDL_stdinc.h>
#include "Command.h"

namespace dae
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		ButtonUp,
		ButtonLeft,
		ButtonDown,
		ButtonRight,
		L1,
		R1
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		InputManager(InputManager const& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(InputManager const& rhs) = delete;
		InputManager& operator=(InputManager&& rhs) = delete;
		~InputManager();
		
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
		
		void AddCommand(SDL_Keycode code, Command* pCommand);
		void SetUseKeyboard(bool bUseKb) { m_bUseKeyboard = bUseKb; }
	private:
		XINPUT_STATE m_CurrentControllerState{};
		
		std::map<int, Command*> m_CommandMap;

		bool m_bUseKeyboard{false};
		
		bool ProcessControllerInput();
		bool ProcessKeyboardInput();
	};
}
