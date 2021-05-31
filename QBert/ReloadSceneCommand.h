#pragma once
#include "Command.h"

class QBertScene;
class ReloadSceneCommand final : public empire::Command
{
public:
	ReloadSceneCommand(QBertScene* pScene);
	ReloadSceneCommand* Clone() override { return new ReloadSceneCommand(*this); }
	ReloadSceneCommand(ReloadSceneCommand&& other) = delete;
	ReloadSceneCommand& operator=(ReloadSceneCommand const& rhs) = delete;
	ReloadSceneCommand& operator=(ReloadSceneCommand&& rhs) = delete;
	~ReloadSceneCommand() = default;

	void Execute() override;

private:
	QBertScene* m_pScene;

	ReloadSceneCommand(ReloadSceneCommand const& other);
};