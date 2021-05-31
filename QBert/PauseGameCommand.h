#pragma once
#include "Command.h"

namespace empire
{
	class GameObject;
}

class QBertScene;
class PauseGameCommand final : public empire::Command
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
	empire::GameObject* m_pPauseMenu;
	
	PauseGameCommand(PauseGameCommand const& other);
};