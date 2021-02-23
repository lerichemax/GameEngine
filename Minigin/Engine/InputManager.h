#pragma once
#include <XInput.h>
#include "Singleton.h"

#include <SDL.h>
#include <map>
#include <SDL_stdinc.h>

class Command;
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
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;

		void AddCommand(SDL_Keycode code, Command* pCommand);
	private:
		XINPUT_STATE m_CurrentControllerState{};
		Uint8 const* m_pLastKeyboardState{};
		Uint8 const* m_pCurrentKeyboardState{};
		
		std::map<int, Command*> m_CommandMap;

		bool ProcessControllerInput();
		bool ProcessKeyboardInput();
	};
}
