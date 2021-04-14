#pragma once

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

namespace empire
{
	class Command
	{
	public:
		Command(KeyActionState state) : m_KeyAction{ state }, m_LastKeyPosition{ LastKeyPosition::up} {}
		Command(Command const& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(Command const& rhs) = delete;
		Command& operator=(Command&& rhs) = delete;
		virtual ~Command() = default;

		virtual void Execute() = 0;
		KeyActionState GetKeyActionState() const { return m_KeyAction; }
		LastKeyPosition GetLastKeyPosition() const { return m_LastKeyPosition; }
		void SetLastKeyPosition(LastKeyPosition lks) { m_LastKeyPosition = lks; }

	protected:
		KeyActionState m_KeyAction;
		LastKeyPosition m_LastKeyPosition;
	};
}

