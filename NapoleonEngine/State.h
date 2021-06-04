#pragma once

namespace empire
{
	class GameObject;
	class State
	{
	public:
		State() = default;
		virtual ~State() = default;
		
		virtual State* Update(GameObject* pObject) = 0;
		virtual void HandleInput(GameObject* pObject) =0;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
	};
}
