#pragma once
#include "Command.h"


class GameObject;
class QBertScene;
class PauseGameCommand final : public Command
{
public:
	PauseGameCommand(QBertScene* pScene, GameObject* pPauseMenu);
	PauseGameCommand* Clone() override { return new PauseGameCommand(*this); }
	PauseGameCommand(PauseGameCommand&& other) = delete;
	PauseGameCommand& operator=(PauseGameCommand const& rhs) = delete;
	PauseGameCommand& operator=(PauseGameCommand&& rhs) = delete;
	~PauseGameCommand() = default;
	
	void Execute() override;

private:
	QBertScene* m_pScene;
	GameObject* m_pPauseMenu;
	
	PauseGameCommand(PauseGameCommand const& other);
};