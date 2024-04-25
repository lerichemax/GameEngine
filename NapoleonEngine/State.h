#pragma once

class GameObject;
class State
{
public:
	State() = default;
	State(State const& other) = delete;
	State(State&& other) = delete;
	State& operator=(State const& rhs) = delete;
	State& operator=(State&& rhs) = delete;
	virtual ~State() = default;
		
	virtual State* Update(GameObject* pObject) = 0;
	virtual void HandleInput(GameObject* pObject) =0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;
};
