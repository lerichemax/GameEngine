#pragma once
#include "Command.h"

class QBertScene;
class ReloadSceneCommand final : public Command
{
public:
	ReloadSceneCommand(QBertScene* pScene);
	ReloadSceneCommand* Clone() override { return new ReloadSceneCommand(*this); }
	ReloadSceneCommand(ReloadSceneCommand&& other) = delete;
	ReloadSceneCommand& operator=(ReloadSceneCommand const& rhs) = delete;
	ReloadSceneCommand& operator=(ReloadSceneCommand&& rhs) = delete;
	~ReloadSceneCommand() = default;

	void Execute() override;

	void Serialize(StreamWriter& writer) const override {};
	void Deserialize(JsonReader const* reader, SerializationMap& context) override {};

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};

private:
	QBertScene* m_pScene;

	ReloadSceneCommand(ReloadSceneCommand const& other);
};