#pragma once
#include <XInput.h>
#include "Singleton.h"

#include "SDL.h"
#include <map>
#include "Command.h"

namespace empire
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
		
		InputManager(InputManager const& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(InputManager const& rhs) = delete;
		InputManager& operator=(InputManager&& rhs) = delete;
		~InputManager();
		
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
		
		void AddCommand(SDL_Keycode code, Command* pCommand);

		glm::vec2 GetMousePosition() const { return m_MousePosition; }
		bool IsMousePressed() const { return m_bIsMouseClicked; }
	private:
		friend class Singleton<InputManager>;
		InputManager();
		
		XINPUT_STATE m_CurrentControllerState{};
		
		std::map<int, Command*> m_CommandMap;
		
		glm::vec2 m_MousePosition;
		
		bool m_bIsMouseClicked; //left button

		void ProcessControllerInput();
		bool ProcessKeyboardInput();
		
	};
}
